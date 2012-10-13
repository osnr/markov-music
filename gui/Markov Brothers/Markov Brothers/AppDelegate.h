//
//  AppDelegate.h
//  Markov Brothers
//
//  Created by Jan Dlabal on 10/13/12.
//  Copyright (c) 2012 Jan Dlabal. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate> {
    NSArray *fileNames;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSTextField *selectedFiles;
@property (assign) IBOutlet NSTextField *inputField;
@property (assign) IBOutlet NSTextField *outputField;
@property (assign) IBOutlet NSTextField *fuzz;
@property (assign) IBOutlet NSTextField *order;
@property (assign) IBOutlet NSButton *unify;

- (IBAction)run:(id)sender;
- (IBAction)play:(id)sender;
- (IBAction)selectInput:(id)sender;

@end
