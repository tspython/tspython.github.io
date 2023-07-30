<!DOCTYPE html>
<html>
<head>
  <title>Tushar's Projects</title>
  <link rel="stylesheet" type="text/css" href="styles.css">
  <style>
    body {
      min-height: 100vh;
      margin: 0;
    }

    .header-container {
      display: flex;
      justify-content: center;
      align-items: center;
      padding: 0px 0;
      position: fixed;
      top: 0;
      width: 100%;
      background-color: white;
      z-index: 100;
      margin-left: 5px;
      margin-right: -6px;
    }

    .container {
      display: flex;
      flex-direction: column;
      align-items: center;
      text-align: center;
      max-width: 800px;
      margin: 0 auto;
      padding-top: 60px;
    }
    .navigation li {
      display: inline-block;
      margin-right: 20px;
    }

    /* Define the dot styles for different programming languages */
    .dot {
      height: 10px;
      width: 10px;
      border-radius: 50%;
      display: inline-block;
      margin-right: 5px;
    }

    .kotlin { background-color: purple; }
    .rust { background-color: red; }
    .typescript { background-color: darkblue; }
    .go { background-color: lightblue; }
    .bash { background-color: lightgreen; }
    .cpp { background-color: pink; }
  </style>
</head>
<body>
  <div class="container">
    <!-- Header Container -->
    <div class="header-container">
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
    </div>

    <main>

      <!-- Project: puck -->
      <h2>puck</h2>
      <p>
        Powerful media server application for downloading and reading manga. 
        <span class="dot kotlin"></span>Kotlin
        <span class="dot rust"></span>Rust
        <span class="dot typescript"></span>TypeScript
      </p>

      <!-- Project: teleMemeBot -->
      <h2>teleMemeBot</h2>
      <p>
        A project written in Go and Bash script. 
        <span class="dot go"></span>Go
        <span class="dot bash"></span>Bash
      </p>

      <!-- Project: alohware -->
      <h2>alohware</h2>
      <p>
        A project written in C++. 
        <span class="dot cpp"></span>C++
      </p>

      <!-- Project: imagePro -->
      <h2>imagePro</h2>
      <p>
         Rust Image Processing CLI 
        <span class="dot rust"></span>Rust
      </p>
    </main>
  
    <footer>
      <p>&copy; 2023 Tushar Shrivastav</p>
    </footer>
  </div>
</body>
</html>
