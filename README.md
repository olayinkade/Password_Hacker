# Introdution
This algorithm takes a hashcode and tries a brute force approach to determine what the the actual text that generated the hashcode. It creates a number of threads and simultaneously tries to solve different hashcodes. This code uses locks to make sure that threads are not accessing the same memory. It reads the hashcode from passwords.txt
# Setup
1. To Compile run ``make``
2. To run ``./Hacker``
