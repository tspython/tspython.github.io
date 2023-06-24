<?php
  $name = $_POST['name'];
  $email = $_POST['email'];
  $message = $_POST['message'];

  // Set up the email headers
  $to = "tshrivastav@scu.edu";
  $subject = "Contact Form Submission";
  $headers = "From: " . $email . "\r\n" .
             "Reply-To: " . $email . "\r\n" .
             "X-Mailer: PHP/" . phpversion();

  // Compose the email message
  $email_message = "Name: " . $name . "\r\n" .
                   "Email: " . $email . "\r\n" .
                   "Message: " . $message;

  // Send the email
  $mail_sent = mail($to, $subject, $email_message, $headers);

  if ($mail_sent) {
    // Redirect the user to a thank you page
    header("Location: thank-you.html");
    exit();
  } else {
    // Handle the case where the email could not be sent
    echo "Oops! Something went wrong. Please try again.";
  }
?>

