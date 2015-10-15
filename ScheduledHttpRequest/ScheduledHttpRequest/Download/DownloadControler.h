//
//  DownloadControler.h
//  ScheduledHttpRequest
//
//  Created by HAMIDOUCHE Lyes on 02/10/2015.
//  Copyright © 2015 HAMIDOUCHE Lyes. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AFNetworking/AFNetworking.h>
#import "MyCustomRequest.h"
@interface DownloadControler : NSObject


//-(NSURLSessionDownloadTask*) createRequest:(NSString *) url;
-(MyCustomRequest *) createRequestWithURL:(NSString *)url andStartDate:(NSDate*)date;
- (MyCustomRequest *)createRequestWithURL:(NSString *)url;


@end
