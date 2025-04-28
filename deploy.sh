#!/bin/bash

set -e

echo "Starting blog generation..."

gcc -o blog_generator blog_generator.c

./blog_generator

echo "Blog generation complete."

if [ -n "$GITHUB_ACTIONS" ]; then
    echo "Running in GitHub Actions environment, staging changes..."
    git config --global user.name "GitHub Actions"
    git config --global user.email "actions@github.com"
    git add blogs/*.html blog.html
    git commit -m "Automatically generated blog posts" || echo "No changes to commit"
    git push
fi

echo "Deployment script completed successfully." 