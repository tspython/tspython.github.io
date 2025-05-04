#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>

#define MAX_PATH 1024
#define MAX_LINE 4096
#define MAX_TITLE 256
#define MAX_DATE 64
#define MAX_SUMMARY 512
#define MAX_BLOGS 100

typedef struct {
    char filename[MAX_PATH];
    char title[MAX_TITLE];
    char date[MAX_DATE];
    char datetime[MAX_DATE];
    char summary[MAX_SUMMARY];
} BlogPost;

void ensure_directory_exists(const char *path);
void parse_markdown_metadata(const char *md_path, BlogPost *post);
void convert_markdown_to_html(const char *md_path, const char *html_path, const BlogPost *post);
void update_blog_index(BlogPost *posts, int count);
int compare_blog_posts(const void *a, const void *b);
char *trim(char *str);

int main() {
    DIR *dir;
    struct dirent *entry;
    BlogPost posts[MAX_BLOGS];
    int post_count = 0;
    char content_dir[] = "content/blogs";
    char output_dir[] = "blogs";
    
    ensure_directory_exists(output_dir);
    
    dir = opendir(content_dir);
    if (dir == NULL) {
        printf("Error: Could not open directory %s\n", content_dir);
        return 1;
    }
    
    while ((entry = readdir(dir)) != NULL && post_count < MAX_BLOGS) {
        char md_path[MAX_PATH];
        char html_path[MAX_PATH];
        char *ext;
        
        if (entry->d_name[0] == '.' || 
            (ext = strrchr(entry->d_name, '.')) == NULL || 
            strcmp(ext, ".md") != 0) {
            continue;
        }
        
        snprintf(md_path, MAX_PATH, "%s/%s", content_dir, entry->d_name);
        
        char basename[MAX_PATH];
        strncpy(basename, entry->d_name, ext - entry->d_name);
        basename[ext - entry->d_name] = '\0';
        
        snprintf(html_path, MAX_PATH, "%s/%s.html", output_dir, basename);
        
        parse_markdown_metadata(md_path, &posts[post_count]);
        
        snprintf(posts[post_count].filename, MAX_PATH, "%s", basename);
        
        convert_markdown_to_html(md_path, html_path, &posts[post_count]);
        
        printf("Converted %s to %s\n", md_path, html_path);
        post_count++;
    }
    
    closedir(dir);
    
    if (post_count > 0) {
        qsort(posts, post_count, sizeof(BlogPost), compare_blog_posts);
        
        update_blog_index(posts, post_count);
    }
    
    printf("Blog generation complete. %d posts processed.\n", post_count);
    return 0;
}

// Ensure a directory exists
void ensure_directory_exists(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
    }
}

// Parse markdown file for metadata
void parse_markdown_metadata(const char *md_path, BlogPost *post) {
    FILE *file = fopen(md_path, "r");
    if (!file) {
        printf("Error: Could not open %s\n", md_path);
        return;
    }
    
    char line[MAX_LINE];
    int in_metadata = 0;
    
    strcpy(post->title, "Untitled");
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(post->date, MAX_DATE, "%d %b, %Y", t);
    strftime(post->datetime, MAX_DATE, "%Y-%m-%d", t);
    
    strcpy(post->summary, "");
    
    while (fgets(line, MAX_LINE, file)) {
        char *trimmed = trim(line);
        
        if (strcmp(trimmed, "---") == 0) {
            if (in_metadata) {
                break;
            } else {
                in_metadata = 1;
                continue;
            }
        }
        
        if (in_metadata) {
            char *key = strtok(line, ":");
            if (key) {
                char *value = strtok(NULL, "\n");
                if (value) {
                    value = trim(value);
                    
                    if (strcasecmp(trim(key), "title") == 0) {
                        strncpy(post->title, value, MAX_TITLE - 1);
                    } else if (strcasecmp(trim(key), "date") == 0) {
                        struct tm tm = {0};
                        char *result = strptime(value, "%Y-%m-%d", &tm);
                        if (result) {
                            strftime(post->date, MAX_DATE, "%d %b, %Y", &tm);
                            strftime(post->datetime, MAX_DATE, "%Y-%m-%d", &tm);
                        }
                    } else if (strcasecmp(trim(key), "summary") == 0) {
                        strncpy(post->summary, value, MAX_SUMMARY - 1);
                    }
                }
            }
        }
    }
    
    if (strlen(post->summary) == 0) {
        rewind(file);
        int found_content = 0;
        
        while (fgets(line, MAX_LINE, file)) {
            if (!found_content && strcmp(trim(line), "---") == 0) {
                found_content = !found_content;
                continue;
            }
            
            if (found_content && strlen(trim(line)) > 0) {
                strncpy(post->summary, trim(line), MAX_SUMMARY - 1);
                break;
            }
        }
    }
    
    fclose(file);
}

// Convert markdown to HTML
void convert_markdown_to_html(const char *md_path, const char *html_path, const BlogPost *post) {
    FILE *md_file = fopen(md_path, "r");
    FILE *html_file = fopen(html_path, "w");
    
    if (!md_file || !html_file) {
        printf("Error: Could not open files for conversion\n");
        if (md_file) fclose(md_file);
        if (html_file) fclose(html_file);
        return;
    }
    
    fprintf(html_file, "<!DOCTYPE html>\n");
    fprintf(html_file, "<html lang=\"en\">\n");
    fprintf(html_file, "<head>\n");
    fprintf(html_file, "    <meta charset=\"UTF-8\">\n");
    fprintf(html_file, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(html_file, "    <link rel=\"icon\" type=\"image/x-icon\" href=\"../images/favicon.ico\">\n");
    fprintf(html_file, "    <title>%s | tushar's blog</title>\n", post->title);
    fprintf(html_file, "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../main.css\">\n");
    fprintf(html_file, "    <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css\">\n");
    fprintf(html_file, "</head>\n");
    fprintf(html_file, "<body>\n");
    fprintf(html_file, "    <canvas id=\"shader-background\"></canvas>\n");
    fprintf(html_file, "    \n");
    fprintf(html_file, "    <header>\n");
    fprintf(html_file, "        <h1>tushar</h1>\n");
    fprintf(html_file, "        <p class=\"subtitle\">programmer</p>\n");
    fprintf(html_file, "        \n");
    fprintf(html_file, "        <div class=\"social-icons\">\n");
    fprintf(html_file, "            <a href=\"https://github.com/tspython/\" title=\"GitHub\"><i class=\"fab fa-github\"></i></a>\n");
    fprintf(html_file, "            <a href=\"https://twitter.com/tspython6535\" title=\"Twitter\"><i class=\"fab fa-twitter\"></i></a>\n");
    fprintf(html_file, "            <a href=\"https://www.shadertoy.com/user/case6535\" title=\"Shadertoy\"><i class=\"fa fa-code\"></i></a>\n");
    fprintf(html_file, "            <a href=\"https://huggingface.co/tspython\" title=\"Hugging Face\">\n");
    fprintf(html_file, "                <img src=\"https://svgl.app/library/hugging_face.svg\" alt=\"Hugging Face\" style=\"width: 16px; height: 16px; vertical-align: middle;\">\n");
    fprintf(html_file, "            </a>\n");
    fprintf(html_file, "        </div>\n");
    fprintf(html_file, "        \n");
    fprintf(html_file, "        <nav>\n");
    fprintf(html_file, "            <a href=\"../index.html\">home</a>\n");
    fprintf(html_file, "            <a href=\"../blog.html\">blog</a>\n");
    fprintf(html_file, "            <a href=\"../nfl_bracket.html\">nfl bracket maker</a>\n");
    fprintf(html_file, "            <a href=\"../graphics.html\">graphics programming</a>\n");
    fprintf(html_file, "        </nav>\n");
    fprintf(html_file, "    </header>\n");
    fprintf(html_file, "\n");
    fprintf(html_file, "    <main>\n");
    fprintf(html_file, "        <section class=\"section\">\n");
    fprintf(html_file, "            <h2 class=\"section-title\">%s</h2>\n", post->title);
    fprintf(html_file, "            <div class=\"blog-date\">\n");
    fprintf(html_file, "                <time datetime=\"%s\">%s</time>\n", post->datetime, post->date);
    fprintf(html_file, "            </div>\n");
    fprintf(html_file, "            <div class=\"blog-content\">\n");
    
    char line[MAX_LINE];
    int in_metadata = 0;
    int skip_metadata = 1;
    
    while (fgets(line, MAX_LINE, md_file)) {
        if (strcmp(trim(line), "---") == 0) {
            in_metadata = !in_metadata;
            continue;
        }
        
        if (in_metadata || skip_metadata) {
            if (!in_metadata) {
                skip_metadata = 0;
            }
            continue;
        }
        
        char *trimmed = trim(line);
        
        if (strlen(trimmed) == 0) {
            fprintf(html_file, "                <p></p>\n");
        } else if (trimmed[0] == '#') {
            int level = 1;
            while (trimmed[level] == '#' && level < 6) level++;
            fprintf(html_file, "                <h%d>%s</h%d>\n", 
                    level, trim(trimmed + level), level);
        } else if (strncmp(trimmed, "- ", 2) == 0) {
            fprintf(html_file, "                <ul><li>%s</li></ul>\n", trim(trimmed + 2));
        } else if (isdigit(trimmed[0]) && strncmp(trimmed + 1, ". ", 2) == 0) {
            fprintf(html_file, "                <ol><li>%s</li></ol>\n", trim(trimmed + 3));
        } else {
            fprintf(html_file, "                <p>%s</p>\n", trimmed);
        }
    }
    
    fprintf(html_file, "            </div>\n");
    fprintf(html_file, "        </section>\n");
    fprintf(html_file, "    </main>\n");
    fprintf(html_file, "    \n");
    fprintf(html_file, "    <script src=\"../shader-background.js\"></script>\n");
    fprintf(html_file, "</body>\n");
    fprintf(html_file, "</html>\n");
    
    fclose(md_file);
    fclose(html_file);
}

// Update the blog index page
void update_blog_index(BlogPost *posts, int count) {
    FILE *index_file = fopen("blog.html", "w");
    if (!index_file) {
        printf("Error: Could not open blog.html for writing\n");
        return;
    }
    
    fprintf(index_file, "<!DOCTYPE html>\n");
    fprintf(index_file, "<html lang=\"en\">\n");
    fprintf(index_file, "<head>\n");
    fprintf(index_file, "    <meta charset=\"UTF-8\">\n");
    fprintf(index_file, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(index_file, "    <link rel=\"icon\" type=\"image/x-icon\" href=\"images/favicon.ico\">\n");
    fprintf(index_file, "    <title>tushar's blog</title>\n");
    fprintf(index_file, "    <link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\">\n");
    fprintf(index_file, "    <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css\">\n");
    fprintf(index_file, "</head>\n");
    fprintf(index_file, "<body>\n");
    fprintf(index_file, "    <canvas id=\"shader-background\"></canvas>\n");
    fprintf(index_file, "    \n");
    fprintf(index_file, "    <header>\n");
    fprintf(index_file, "        <h1>tushar</h1>\n");
    fprintf(index_file, "        <p class=\"subtitle\">programmer</p>\n");
    fprintf(index_file, "        \n");
    fprintf(index_file, "        <div class=\"social-icons\">\n");
    fprintf(index_file, "            <a href=\"https://github.com/tspython/\" title=\"GitHub\"><i class=\"fab fa-github\"></i></a>\n");
    fprintf(index_file, "            <a href=\"https://twitter.com/tspython6535\" title=\"Twitter\"><i class=\"fab fa-twitter\"></i></a>\n");
    fprintf(index_file, "            <a href=\"https://www.shadertoy.com/user/case6535\" title=\"Shadertoy\"><i class=\"fa fa-code\"></i></a>\n");
    fprintf(index_file, "            <a href=\"https://huggingface.co/tspython\" title=\"Hugging Face\">\n");
    fprintf(index_file, "                <img src=\"https://svgl.app/library/hugging_face.svg\" alt=\"Hugging Face\" style=\"width: 16px; height: 16px; vertical-align: middle;\">\n");
    fprintf(index_file, "            </a>\n");
    fprintf(index_file, "        </div>\n");
    fprintf(index_file, "        \n");
    fprintf(index_file, "        <nav>\n");
    fprintf(index_file, "            <a href=\"index.html\">home</a>\n");
    fprintf(index_file, "            <a href=\"blog.html\">blog</a>\n");
    fprintf(index_file, "            <a href=\"nfl_bracket.html\">nfl bracket maker</a>\n");
    fprintf(index_file, "            <a href=\"graphics.html\">graphics programming</a>\n");
    fprintf(index_file, "        </nav>\n");
    fprintf(index_file, "    </header>\n");
    fprintf(index_file, "\n");
    fprintf(index_file, "    <main>\n");
    fprintf(index_file, "        <section class=\"section\">\n");
    fprintf(index_file, "            <h2 class=\"section-title\">blog posts</h2>\n");
    
    for (int i = 0; i < count; i++) {
        fprintf(index_file, "            \n");
        fprintf(index_file, "            <div class=\"blog-post\">\n");
        fprintf(index_file, "                <div class=\"blog-date\">\n");
        fprintf(index_file, "                    <time datetime=\"%s\">%s</time>\n", 
                posts[i].datetime, posts[i].date);
        fprintf(index_file, "                </div>\n");
        fprintf(index_file, "                <h3 class=\"project-title\">\n");
        fprintf(index_file, "                    <a href=\"blogs/%s.html\">%s</a>\n", 
                posts[i].filename, posts[i].title);
        fprintf(index_file, "                </h3>\n");
        fprintf(index_file, "                <p>%s</p>\n", posts[i].summary);
        fprintf(index_file, "                <a href=\"blogs/%s.html\" class=\"read-more\">read more</a>\n", 
                posts[i].filename);
        fprintf(index_file, "            </div>\n");
    }
    
    fprintf(index_file, "        </section>\n");
    fprintf(index_file, "    </main>\n");
    fprintf(index_file, "    \n");
    fprintf(index_file, "    <script src=\"shader-background.js\"></script>\n");
    fprintf(index_file, "</body>\n");
    fprintf(index_file, "</html>\n");
    
    fclose(index_file);
    printf("Updated blog index with %d posts\n", count);
}

// Compare function for sorting blog posts by date (newest first)
int compare_blog_posts(const void *a, const void *b) {
    const BlogPost *post_a = (const BlogPost *)a;
    const BlogPost *post_b = (const BlogPost *)b;
    
    return -strcmp(post_a->datetime, post_b->datetime);
}

// Utility to trim whitespace from a string
char *trim(char *str) {
    if (!str) return NULL;
    
    while(isspace((unsigned char)*str)) str++;
    
    if(*str == 0) return str;
    
    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    *(end+1) = 0;
    
    return str;
} 