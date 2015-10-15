//
//  MyCustomRequest.m
//  ScheduledHttpRequest
//
//  Created by HAMIDOUCHE Lyes on 05/10/2015.
//  Copyright © 2015 HAMIDOUCHE Lyes. All rights reserved.
//

#import "MyCustomRequest.h"
#import <AFNetworking/AFNetworking.h>
@implementation MyCustomRequest

double expectedContentLength;
double currentlyDownloadedData;
double fractionCompleted;
static void *ProgressObserverContext = &ProgressObserverContext;
/**
 * Create a NSURLSessionDownloadTask
 */

-(instancetype)initWithUrlString:(NSString*)url andStartDate:(NSDate*)start {
    self = [super init];
    if (self) {
        // First step: create the structure
        self.urlString = url;
        self.startDate = start;
        NSTimeInterval  ti = start.timeIntervalSinceNow;
        [NSTimer scheduledTimerWithTimeInterval:ti target:self selector:@selector(startDownload:) userInfo:nil repeats:false];
    }
    return self;
}
- (void) startDownload:(id) sender {
    NSURL *url = [NSURL URLWithString:self.urlString];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    AFHTTPSessionManager *session = [AFHTTPSessionManager manager];
    NSProgress *progress;
    self.startDate = [NSDate date];
    self.downloadTask = [session downloadTaskWithRequest:request progress:&progress destination:^NSURL *(NSURL *targetPath, NSURLResponse *response) {
        NSURL *documentsDirectoryURL = [[NSFileManager defaultManager] URLForDirectory:NSDocumentDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:NO error:nil];
        expectedContentLength = response.expectedContentLength; // Content length
        NSLog(@"Expected content length for %@ : %ld  bytes",url.path,(long)expectedContentLength);
        self.filename = [response suggestedFilename];
        self.filepath =[documentsDirectoryURL URLByAppendingPathComponent:[response suggestedFilename]].path;
        return [documentsDirectoryURL URLByAppendingPathComponent:[response suggestedFilename]];
    }  completionHandler:^(NSURLResponse *response, NSURL *filePath, NSError *error) {
        
        [progress removeObserver:self forKeyPath:@"fractionCompleted" context:NULL];
        if (error.code ==0) {
            self.endDate = [NSDate date];
            self.elaplsedTime = [self.endDate timeIntervalSinceDate:self.startDate];
            NSLog(@"Download of %@ Finished in %.2f seconds",self.filename,self.elaplsedTime);
            NSString *txt = [NSString stringWithFormat:@"%.5f\n",self.elaplsedTime];
            NSString *path = [NSString stringWithFormat:@"%@.times",self.filepath];
            if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
                [[NSFileManager defaultManager] createFileAtPath:path contents:[txt dataUsingEncoding:NSUTF8StringEncoding] attributes:nil];
            } else {
                NSFileHandle *fileHandle = [NSFileHandle fileHandleForWritingAtPath:path];
                [fileHandle seekToEndOfFile];
                [fileHandle writeData:[txt dataUsingEncoding:NSUTF8StringEncoding]];
                [fileHandle closeFile];
            }
        }
    }];
    [self.downloadTask resume];
    [progress addObserver:self
               forKeyPath:@"fractionCompleted"
                  options:NSKeyValueObservingOptionNew
                  context:NULL];
    
    currentlyDownloadedData = 0;
    [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(downloadSpeed:) userInfo:nil repeats:YES];
    
}
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object
                        change:(NSDictionary *)change context:(void *)context
{
    if ([keyPath isEqualToString:@"fractionCompleted"] ){
        NSProgress *p = (NSProgress *) object;
        //NSLog(@"Downloading %@ to progress %.2f",self.urlString,(p.fractionCompleted*100));
        fractionCompleted = p.fractionCompleted;
    }
}
// Create a request and start it
-(instancetype)initAndStartWithUrlString:(NSString*)urlStr {
    self = [super init];
    if (self) {
        // First step: create the structure
        self.urlString = urlStr;
        self.startDate = [NSDate date];
        NSURL *url = [NSURL URLWithString:self.urlString];
        NSURLRequest *request = [NSURLRequest requestWithURL:url];
        AFHTTPSessionManager *session = [AFHTTPSessionManager manager];
        NSProgress *progress;
        self.startDate = [NSDate date];
        self.downloadTask = [session downloadTaskWithRequest:request progress:&progress destination:^NSURL *(NSURL *targetPath, NSURLResponse *response) {
            expectedContentLength = (long) response.expectedContentLength;
            NSURL *documentsDirectoryURL = [[NSFileManager defaultManager] URLForDirectory:NSDocumentDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:NO error:nil];
            self.filename = [response suggestedFilename];
            self.filepath =[documentsDirectoryURL URLByAppendingPathComponent:[response suggestedFilename]].path;
            return [documentsDirectoryURL URLByAppendingPathComponent:[response suggestedFilename]];
        }  completionHandler:^(NSURLResponse *response, NSURL *filePath, NSError *error) {
            
            [progress removeObserver:self forKeyPath:@"fractionCompleted" context:NULL];
            if (error.code ==0) {
                self.endDate = [NSDate date];
                self.elaplsedTime = [self.endDate timeIntervalSinceDate:self.startDate];
                NSLog(@"Download of %@ Finished in %.2f seconds",self.filename,self.elaplsedTime);
                NSString *txt = [NSString stringWithFormat:@"%.5f\n",self.elaplsedTime];
                NSString *path = [NSString stringWithFormat:@"%@.times",self.filepath];
                if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
                    [[NSFileManager defaultManager] createFileAtPath:path contents:[txt dataUsingEncoding:NSUTF8StringEncoding] attributes:nil];
                } else {
                    NSFileHandle *fileHandle = [NSFileHandle fileHandleForWritingAtPath:path];
                    [fileHandle seekToEndOfFile];
                    [fileHandle writeData:[txt dataUsingEncoding:NSUTF8StringEncoding]];
                    [fileHandle closeFile];
                }
            }
        }];
        [self.downloadTask resume];
        [progress addObserver:self
                   forKeyPath:@"fractionCompleted"
                      options:NSKeyValueObservingOptionNew
                      context:NULL];
        currentlyDownloadedData = 0;
        [NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(downloadSpeed:) userInfo:nil repeats:YES];
    }
    return self;
}


#pragma Download Speed
#define TO_MB_DIVIDER 1047552
- (void)downloadSpeed:(id) sender {
    NSTimer * timer = (NSTimer *) sender;
    NSInteger  oldDownload = currentlyDownloadedData;
    currentlyDownloadedData = expectedContentLength * fractionCompleted;
    NSLog(@"Download Speed: %.2f",(currentlyDownloadedData - oldDownload)/TO_MB_DIVIDER);
    
    if (fractionCompleted ==1) {
        [timer invalidate];
    }
}
@end
