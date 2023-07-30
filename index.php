<!DOCTYPE html>
<html>
<head>
  <title>Tushar Shrivastav</title>
  <link rel="stylesheet" type="text/css" href="styles.css">
  <style>
    footer {
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
  <div style="text-align: center;" class>
    <img src="./graphix/IMG_2989.jpg" alt="Tushar Shrivastav" width="200" height="200">
  </div>
  <div style="text-align: center;">
    <a href="https://www.linkedin.com/in/tushar-shrivastav-1b1b3a1b0/"><img src="https://cdn-icons-png.flaticon.com/512/174/174857.png" alt="LinkedIn" width="50" height="50"></a>
    <a href="https://twitter.com/tspython6535"><img src="https://cdn-icons-png.flaticon.com/512/733/733579.png" alt="Twitter" width="50" height="50"></a>
    <a href="https://github.com/tspython"><img src="./graphix/github-mark.svg"alt="Github"width="50"height="50"></a>
  </div>
  <div style="text-align: center;">
    <div style="display: inline-block; text-align: left;" class = "experience">
      <p><strong>Who are you?</strong></p>
      <p>I am Tushar Shrivastav, currently I am completing B.S in Computer Science at Santa Clara University.</p>
      <p>I am also a researcher at <a href= "https://www.scu.edu/engineering/faculty/shaghaghi-navid/epic-lab/">EPIC Labs</a>, doing IoT research to automate agriculture.</p>
      <p>My future plans are to pursue a PhD program in Computer Science focusing on Computer Graphics research with a focus in AR/VR.</p>
    </div>
  </div>
  <div style="text-align: center;">
    <div style="display: inline-block; text-align: left;" class="experience">
      <p><strong>Experience</strong></p>
      <div class = "experience">
        <img src="./graphix/epic_iot.jpeg" alt="Company Logo 1" width="500" height="100">
        <p>2021 - Present</p>
        <p><strong>Researcher</strong></p>
        <p>Working on agricultural IoT Products: DOxy, Hydration Automation. As well as ÂB an Energy Aware Communication Protocol (EACP).</p>
      </div>
      <div>
        <img src="./graphix/realtor.png" alt="Company Logo 2" width="100" height="100">
        <p><img src="./graphix/Realtor.com_logo.png" alt="realtor.com" width="250" height ="50"></p>
        <p>2022 - 2023</p>
        <p><strong>Co-op Software Engineer</strong></p>
        <p>Worked on Full Stack & Cloud Security Development. Improved the security of APIs utilizing OAuth 2.0 and Cryptography.</p>
      </div>
    </div>
  </div>  
  <div style="text-align: center;">
    <div style="display: inline-block; text-align: left;">
      <p><strong>Music - Top Albums from past 3 months</strong></p>
      <?php
        $imageURL = 'https://www.tapmusic.net/collage.php?user=camelCaseJava&type=3month&size=4x4&caption=true';
        echo "<img src='$imageURL' alt='Last.fm Weekly Chart' width='400' height='400'>";
      ?>
    </div>
  </div>
  
  <footer>
    <p>&copy; 2023 Tushar Shrivastav</p>
  </footer>
</body>
</html>

