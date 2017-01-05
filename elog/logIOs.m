/** @file
 * @author Edouard DUPIN
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license MPL v2.0 (see license file)
 */


#import <UIKit/UIKit.h>
#include <elog/logIOs.h>

void iosNSLog(const char * _value) {
	NSLog(@"\r%s", _value);
}


