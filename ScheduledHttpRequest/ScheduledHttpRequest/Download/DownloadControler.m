//
//  DownloadControler.m
//  ScheduledHttpRequest
//
//  Created by HAMIDOUCHE Lyes on 02/10/2015.
//  Copyright © 2015 HAMIDOUCHE Lyes. All rights reserved.
//

#import "DownloadControler.h"

@implementation DownloadControler


- (MyCustomRequest *)createRequestWithURL:(NSString *)url andStartDate:(NSDate*) date{
    MyCustomRequest *request = [[MyCustomRequest alloc] initWithUrlString:url andStartDate:date];
    return request;
}

- (MyCustomRequest *)createRequestWithURL:(NSString *)url {
    MyCustomRequest *request = [[MyCustomRequest alloc] initAndStartWithUrlString:url];
    return request;
}

@end
