The Model Editor can import other model formats. These importers
resides in DLLs. To add/remove importers add/remove them from
this folder. A DLL named "EXTENTION.dll" will be the importer
for all models with the extention "EXTENTION".

Writing your own importers requires knowledge in C++. See
the Model Importer API for more info.
