# ADFParser

This is a C99 parser of the .adf file format as described here: http://nsma.org/wp-content/uploads/2016/05/wg16_99_050.pdf

Note: The specification describes some of the fields as required. In the files I have seen, many "required" fields are missing. Keep this in mind when using the data from an .adf file.

## Usage

The API consists of two functions:

    struct ADFPattern parseADFPattern(const char* data, int len)
  This function returns a large struct whose members have all of the properties stored in an .adf file.
  
    void freeADFPattern(struct ADFPattern pattern)
  This function frees a couple of allocated variable length strings within the struct.
 
