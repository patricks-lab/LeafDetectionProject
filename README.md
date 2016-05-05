Leaf Detection Algorithm - Documentation
==============

Hi there! Here is my code for the Leaf Detection algorithm. 
I have included in addition to the source code a few sample leaves to play with or experiment with, that belong to various
datasets. I will leave it to you to figure out which one is which ;)
Also, special thanks to Leafsnap (http://leafsnap.com/dataset/) for images of leaves!!

To try out and run the code, you will need a few prerequisites: 
- OpenCV (Version 2.4.9 or higher - www.opencv.org)
- C++ and Java compiler (gcc, javac, etc.)
- Bash or command line

To start, download the files and cd into the directory.
For example, if you downloaded to Downloads, you can:

    cd ~/Downloads/LeafDetectionProject-master

Then, compile the files that make up the algorithm:

    g++ AbsoluteDistance.cpp -o AbsoluteDistance
    g++ DatabaseToText4.cpp -o DatabaseToText4 `pkg-config --cflags --libs opencv`
    javac graphic.java 
    chmod +x inputimage.sh

(If you run into any problem here whatsoever, it could mean you haven't satisfied the prerequisites!!)

**Note about files:** The AbsoluteDistance handles the comparison of distances, 
DatabaseToText4 for segmenting the leaf and extracting key features (Leaf distance, and Convexity factor), and
InputImage for dealing with the comparison of the query leaf and the database, automating the process by calling the last two functions.

To test the algorithm, try a few leaves:

    ./inputimage.sh testleaf.jpg
    ./inputimage.sh testleaf2.jpg

Once the algorithm finishes the comparison, your command line will show a series of percentage matches for each leaf:

     x percent similarity, leaf cercidiphyllum_japonicum.database
     y percent similarity, leaf castanea_dentata.database
     z percent similarity, leaf betula_alleghaniensis.database
     a percent similarity, leaf acer_ginnala.database

**Note the percentage has specific guidelines to consider it similar or unsimilar.** That is, 40% doesn't mean it is sort of similar, it actually means not *even* close :) See guidelines.md for more info.

Then, it will ask you if it correctly matched the leaf. 

     Was the leaf correctly identified? Y for yes, or N for no.

If you are satisfied with the result, press Y. To train a new leaf into the database, you can use this to your advantage,  and answer no. This will direct you to a message prompting you to give it the right name:

     Then what database should it be added to, or what new database should be created?
     Also, give me the correct name of the leaf


