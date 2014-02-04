#include <stdio.h>
#include "warehouse.h"

int is_filled; 
void create_order(){
  is_filled = 0;
}
void fill(char* name, int quantity){//function to test
  if( warehouse_has_inventory(name, quantity) == 0 ){
    warehouse_remove(name, quantity);
    is_filled = 1;
  }
}
int isFilled(){
  return is_filled;
}
