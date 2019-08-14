/** @file sedcmp.c
 *  @brief A program that calculates the String Eversis Distance between two
 *         Strings
 *
 *
 *  This file is a complete program(main,functions,...) which implements
 *  a program that calculates the String Eversis Distance between two Strings.
 *  String Eversis Distance or SED is the minimum cost of changes that
 *  are made between two Strings in order the two String to be equal.For more
 *  information @see https://en.wikipedia.org/wiki/Levenshtein_distance.
 *
 * This program implements the Levenshtein with two different ways.With
 * recursion @see sedFunction(const char* s1,const char* s2,int l1,int l2)
 * and with arrays as a matrix for the storing of the costs of changes
 * @see sedFunctionDynamicVersion(const char* w1,int l1,const char* w2,int l2).
 *
 *
 *
 *  @version 1.0
 *  @author Valentinos Pariza
 *  @bug No know bugs.
 *
 */


/*Imported libraries*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CHARS_PER_LINE 80
#define FILE_ERROR -1
#define MEMORY_ALLOCATION_ERROR -3
#define NULL_POINTER_ERROR -2

// Functions Prototypes
int getCharSequence(char* fileName,char** charSequence);

int sedFunction(const char* s1,const char* s2,int c1,int c2);

int sedFunctionDynamicVersion(const char* w1,int l1,const char* w2,int l2);

/**
* @brief Takes a line of characters from a file with name fileName
*
* This method takes a file name ,the address of a variable of type char*
* and reads from the file a single line of characters and the address of the
* characters in memory assigns it to the variable.
*
*
*@param fileName the name of the file to be opened and readas pointer to
*       characters
*
*@param charSequence a pointer to a variable which points to characters
*
*@return 0 if the program was excecuted correctly,FILE_ERROR if the file
* could not be opened or doesn't exist,NULL_POINTER_ERROR if the pointer
* to characters passed as an argument for file name is NULL,
* MEMORY_ALLOCATION_ERROR if a problem in allocating memory from heap was
* appeared(like no more space in heap)
*
*/
int getCharSequence(char* fileName,char** charSequence)
{
   if(fileName==NULL)
      return NULL_POINTER_ERROR;

    if(charSequence==NULL)
      return NULL_POINTER_ERROR;

   FILE* filePointer=NULL;

   filePointer=fopen(fileName,"r");

   if(filePointer==NULL)
      return FILE_ERROR;


   int currentSizeOfString=CHARS_PER_LINE;

   char* word=(char*)malloc(sizeof(char)*CHARS_PER_LINE);

   if(word==NULL)
      return MEMORY_ALLOCATION_ERROR;

   char c=' ';
   int i=0;

   *charSequence=word;



   while(!feof(filePointer) && (c=fgetc(filePointer))!='\n')
     {


         if(currentSizeOfString<=i)
         {
            word=(char*)realloc(*charSequence,sizeof(char)*(
            currentSizeOfString*=2));

            if(word==NULL)
               return MEMORY_ALLOCATION_ERROR;

            *charSequence=word;
         }

          (*charSequence)[i]=c;
            i++;
     }



      // include the nul character (='\0')
     charSequence[i++]='\0';

     fclose(filePointer);

     word=(char*)realloc(*charSequence,sizeof(char)*i);

     if(word==NULL)
         return MEMORY_ALLOCATION_ERROR;

      *charSequence=word;


   return 0;
}



/**
*
*@brief Calculates the SED between two Strings
*
* This method calculates the SED distance between two Strings with recursion.
* It uses an algorythm of calculating  how many changes can be done at two
* Strings in order the two Strings to be equal.It tries a delete,an insertion
* and a substitution move by invoking itself until the base condition occurs.
* It takes the minimum number of changes for all the possible combinations of
* moves (deletion,substitution and insertion) and return it.
*
*
*
* @param s1 the first pointer of characters to be used for calculation of the
*        SED
*
* @param s2 the second pointer of characters to be used for calculation of the
*        SED
*
* @return the minimum number of changes that have to be made on String 1
*         in order the two strings to be equal
*
*
*/
int sedFunction(const char* s1,const char* s2,int l1,int l2)
{
   // l1 and l2 is the length of a string so l1-1 and l2-1 are
   // the last elements of the strings

      if(l1<=0)
         return l2;

      if(l2<=0)
         return l1;


      int a = sedFunction(s1,s2,l1-1,l2)+1;

      int b = sedFunction(s1,s2,l1,l2-1)+1;

      int c = sedFunction(s1,s2,l1-1,l2-1)+((s1[l1-1]==s2[l2-1])? 0 : 1);

      int min=(a>c)? c : a;

      return (min>b)? b : min;    // find the min of a,b and c
}



/**
*
*@brief Fast algorythm that calculates the String Evresis Distance of
* two Strings.
*
* This algorythm is a fast algorithm that instead of looking on all the
* possible combinations for the minimum number of changes in the two strings
* it looks straight away for the minimum number of changes.This algorithm is
* based an an algorithm which uses a two dimensional array.It uses an array
* size of l1*l2 (l1 the size of the sequence of characters 1 and l2 the size
* of the characters sequence 2).It uses the first row and column for the
* changes that happen in associating 0 characters from String 1 and l2
* characters from String 2 (first row, for l2 inserts at String 1) and
* l1 characters from String 1 and 0 characters from String 2 (first column
* ,for l1 deletions at String 1).At the next steps,in each cell it calculates
* the less cost that is needed in order the two Strings to be equal based on
* a delete move,insert move and substitution move.Each cell represents the
* characters that have been considered so far by the column and row that they
* belong(i row means i-1 characters of String 1 have been considered & j column
* means j-1 characters of String 1 have been considered .Every examination
* is being made on the i,j elements).When the examination has finished from
* the field (l1,l2) of the array ,then at the position (l1,l2) of the array
* is the number with the minimum moves that have to be made in order the two
* Strings to be equal.
*
* @param w1 the first pointer of characters to be used for calculation of the
*        SED
*
* @param w2 the second pointer of characters to be used for calculation of the
*        SED
*
* @return the minimum number of changes that have to be made on String 1
*         in order the two strings to be equal
*
*/
int sedFunctionDynamicVersion(const char* w1,int l1,const char* w2,int l2)
{
   int array[l1+1][l2+1];
   int i;


   for(i=0;i<=l1;i++)      // initialize the changes by insertion
      array[i][0]=i;       // with 0 characters of first String
                           // and l2 characters of the second String
                           // on the first row

   for(i=0;i<=l2;i++)        // initialize the changes by deleton
      array[0][i]=i;        // with l1 characters of first String
                           // and 0 characters of the second String
                           // on the first column

   for(i=1;i<=l1;i++)
   {

      char c1=w1[i-1];
      // the next character to examine for equality of String 1

      for(int j=1;j<=l2;j++)
      {
         char c2=w2[j-1];
         // the next character to examine for equality of String 2

      // if they are equal consider only the previous changes that needed
      // for previous characters(i-1 and j-1 |Diagonal jump  ).Before the
      // characters i and j are compared
         if(c1==c2)
            array[i][j]=array[i-1][j-1];
         else
         {
            int delete=array[i-1][j]+1;
            // evaluate the cost of delete considering the previous
            // number of changes before the appearance of character i from
            // the String 1 and plus 1 for the deletion (Vertical jump)

            int insertion=array[i][j-1]+1;
            // evaluate the cost of insertion considering the previous
            // number of changes before the appearance of character j from
            // the String 2 and plus 1 for the insertion of a character to
            // String 1  (Horizontal jump)

            int substitution=array[i-1][j-1]+1;
            // evaluate the cost of substitution considering the previous
            // number of changes before the appearance of character i and j
            // from the String 1 and 2 respectively and plus 1 for from
            //  the substitution of a character at String 1  (Diagonal Jump )


            int min=(delete>insertion)? insertion : delete;

            array[i][j]=(min>substitution)? substitution : min;
            // take the minimum cost of the three tha calculated

         }

      }

   }

   // return the last element of the array that considers of the l1
   // characters of the String 1 and the l2 characters of String 2
    return array[l1][l2];

}



/**
* @brief The main method of the program from where all the operations
*        are controlled and all the Error codes are handled.
*
*
* This method is the main method of the program.It takes as arguments two
* file names(two pointers to characters), reads a line from each of them
* (invokes the method @see getCharSequence(char* fileName,char** charSequence))
* and calculates the SED between them (by invoking the method
* sedFunctionDynamicVersion(const char* w1,int l1,const char* w2,int l2)).At
* the end it prints the result or a message for an Error that might occur
* during the excecution of the some of its operations or in a method.
*
*
*
* @param argc the number of arguments given in command line(the name of the
* file which is excecuted,is included in the command line arguments)
*
* @param argv an array of pointers type of char*--> an array of pointers to
*        characters which these characters represent the arguments
*        passed in the program by the coomand line(the name of the
*        file which is excecuted,is included in the command line arguments)
*
* @return 0 if the program ran without any problem else any number that
*        indicates a problem
*
*/
int main(int argc,char* argv[])
{


   char* charSequence1=NULL;
   char* charSequence2=NULL;



   if(argc!=3)
   {
      printf("Error : Not correct number of arguments for files.\n");
      return 0;

   }


   int status=getCharSequence(argv[1],&charSequence1);


   if(status==FILE_ERROR)
      printf("Error : Problems in reading from the file %s.",argv[1]);

   else if(status==NULL_POINTER_ERROR)
      printf("Error : Null file or charSequence was passed in the function.\n");
   else if(status==MEMORY_ALLOCATION_ERROR)
      printf("Error : Memory problems for allocating memory.\n");

   status=getCharSequence(argv[2],&charSequence2);


   if(status==FILE_ERROR)
      printf("Error : Problems in reading from the file %s.",argv[2]);

   else if(status==NULL_POINTER_ERROR)
      printf("Error : Null file or charSequence was passed in the function");
   else if(status==MEMORY_ALLOCATION_ERROR)
      printf("Error : Memory problems for allocating memory.\n");
   else
   {
     printf("Minimum edits required to convert is %d.",
     sedFunctionDynamicVersion(charSequence1,strlen(charSequence1),
     charSequence2,strlen(charSequence2)) );
   }

   printf("\n");


   return 0;
}
