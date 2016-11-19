# jM2converter 1.0.8b-beta
World of Warcraft Universal Model Converter.  
Works using the [jM2lib project](https://github.com/Koward/jM2lib). *Still early prototype.*  

##Usage (example with Frog.m2) :
This is a Java command-line tool.  
You may need to install Java : [Install Java](https://java.com/en/download/help/download_options.xml)  
And setup paths for your environment : [PATH and CLASSPATH](https://docs.oracle.com/javase/tutorial/essential/environment/paths.html)  

Command line : `java -jar jm2converter.jar -in "Frog.m2" -out "FrogConverted.m2" <OPTIONS>`


##Usage with whole folders(only in oppahansi's version)

Command line : `java -jar jm2converter.jar -in "path/to/inputFolder/" -out "path/to/outputfolder/" <OPTIONS>`

The "/" or "\" at the end of paths is important. New option is -f for whole folders. Only in oppahansi's version.

Type `java -jar jm2converter.jar` to get a list of all converting options and two examples ;)

* Beware, when you retroport a model, you always lose some data (the newer client features) and you won't be able to get them back if you up convert it back later.

* The Legion support is still changing. The game is still in Beta so the M2 format changes. Right now it should work up to build 20810.

##Download :
[MediaFire link](http://adf.ly/1TtsYH)  


[MediaFire link](http://www.mediafire.com/download/339o8t1vm4bw641/jM2converter.jar)  -  oppahansi's version with folder support

##What's yet to be implemented :
* BC => LK up-conversion.
* MDX <=> Classic conversion (needs MDX support in jM2lib first)

##Known issues :
* Ribbons are sometimes funky.
* Colors may go weird, like yellow instead of purple.
Report new ones in the GitHub issue tracker !
* Some models with shaders, like FandralFireCat, are not properly displayed in old clients.

##Credits :
* Thanks to all the guys who test this to find bugs ;) !


## Links
* [Github](https://github.com/Koward/jM2converter)