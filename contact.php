
<!DOCTYPE html>
<html>
<head>
  <title>Contact Me</title>
  <link rel="stylesheet" type="text/css" href="styles.css">
  <style>
    h2 {
      text-align: center;
    }
    
    footer {
      text-align: center;
    }

    .form-container {
      text-align: center;
    }
  </style>
</head>
<body>
  <header>
    <nav>
      <ul class="navigation">
        <li><a href="index.php">About</a></li>
        <li><a href="projects.php">Projects</a></li>
        <li><a href="contact.php">Contact</a></li>
        <li><a href="blog.php">Blog</a></li>
      </ul>
    </nav>
  </header>

  <!-- Rest of your website content -->
  <h2>Contact</h2>
  <div class="form-container">
    <form action="process.php" method="POST">
      <label for="name">Name:</label>
      <input type="text" name="name" id="name" required><br>
      
      <label for="email">Email:</label>
      <input type="email" name="email" id="email" required><br>
      
      <label for="message">Message:</label>
      <textarea name="message" id="message" rows="5" required></textarea><br>
      
      <input type="submit" value="Submit">
    </form>
  </div>
  
  <footer>
    <p>&copy; 2023 Tushar Shrivastav</p>
  </footer>
</body>
</html>

