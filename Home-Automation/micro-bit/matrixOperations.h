/**
    Functions for retrieving and setting the LED
    matrix coordinates.
*/

#ifndef __MATRIX_OPERATIONS_
#define __MATRIX_OPERATIONS_


/* function prototypes */
int getMatrixRow(ManagedString message, int message_row_index);
int getMatrixCol(ManagedString message, int message_col_index);
int setMatrixRow(int row);
int setMatrixCol(int col);


/* function definitions */
int getMatrixRow(ManagedString message, int message_row_index){
    return (int)(message.charAt(message_row_index));
}//end getMatrixRow()

int getMatrixCol(ManagedString message, int message_col_index){
    return (int)(message.charAt(message_col_index));
}//end getMatrixCol()

int setMatrixRow(int row){
    return row - '0';    
}//end setMatrixRow()

int setMatrixCol(int col){
    return col - '0';    
}//end setMatrixCol()


#endif