//
//  MyCustomRequest.h
//  ScheduledHttpRequest
//
//  Created by HAMIDOUCHE Lyes on 05/10/2015.
//  Copyright © 2015 HAMIDOUCHE Lyes. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MyCustomRequest : NSObject

@property NSDate* startDate; // Start date
@property NSDate* endDate; // End date
@property NSString* urlString; // Url string
@property NSURLSessionDownloadTask* downloadTask; // AFNetworking download task object
@property NSString* filename;
@property NSString* filepath;
@property NSTimeInterval elaplsedTime; //  Elaplsed time


// This function is used to  run a http request at a given hour
-(instancetype)initWithUrlString:(NSString*)url andStartDate:(NSDate*)start;

// Create a http downlaod request and start it immediately after creating it.
-(instancetype)initAndStartWithUrlString:(NSString*)url;

// TODO: Add battery and activity tracker in order to follow these states in order to be aware of battery consumption


@end
