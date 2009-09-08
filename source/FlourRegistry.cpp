/** \brief Flour
*/

#include "Flour.h"

// Tools of Flour
#include "FlourToolVersion.h"
#include "FlourToolConvert.h"

void Flour::registerTools()
{
 registerTool("version", new FlourVersion());
 registerTool("convex", new FlourConvert(FlourConvert::ConversionType_Convex));
}
