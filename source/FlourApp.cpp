/** \brief Flour
*/

#include "Flour.h"

int main(int argc, char **argv) 
{
 Flour* flour = new Flour();
 flour->fromCommandLine(argc, argv);
 delete flour;
}
