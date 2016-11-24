This tool allows the user to resize a model by a given value. 

* 1 = equal 
* 0.5 = 50% of the size 
* 2 = 200% of the size 

This tool does not resize Animation coords! In this case you must use resizer.py. Open it up and edit 3 lines:

1. The way to model m2 = M2File (Specify your path):

    m2 = M2File("E:\\Tools\\V50pymodeleditor\\pymodeleditor\\16.m2")

2. Path to saved model(Specify your path):

	m2.write("E:\\Tools\\V50pymodeleditor\\pymodeleditor\\16.m2")

3. Amount to be changed resize factor:

	scale = 0.02

4. Save and close resizer.py

## Credits
Tigurius