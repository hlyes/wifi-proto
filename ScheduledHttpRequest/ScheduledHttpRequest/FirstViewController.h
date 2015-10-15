//
//  FirstViewController.h
//  ScheduledHttpRequest
//
//  Created by HAMIDOUCHE Lyes on 02/10/2015.
//  Copyright © 2015 HAMIDOUCHE Lyes. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <CocoaAsyncSocket/AsyncSocket.h>
#import "DownloadControler.h"
@interface FirstViewController : UIViewController <AsyncSocketDelegate>

@property AsyncSocket *asyncSocket; // Asynchronous socket
@property DownloadControler* downloadController; // DownloadController
@end

