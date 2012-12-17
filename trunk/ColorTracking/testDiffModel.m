function [] = testDiffModel() 

% The file test different ability of modules
% Author:
%       IMS@SCUT Once
%       WMDZTSXCDH

info = imaqhwinfo
winfo = imaqhwinfo('winvideo')
obj = videoinput( 'winvideo' , 1 ,  'YUY2_320x240' ); %'RGB24_320x240' );
himage = preview( obj );
video = figure(1);
set( obj , 'ReturnedColorSpace' , 'rgb' );

%getCapturer paramaters:           %'default_red' | 'getpts' | 'getrect'
capturer = getCapturer( obj , 'getpts' );

%sphere paramater:
%                                   %varargin{ 3 } may be : 'a' 'b' 'c' 'avg' preOfavg 
tk = Tracker( 'sphere' , capturer , 1.5 ); 
tk.showBoundary();
showSupportSpace( obj , tk , 1 );  % third 1 is the step
%

%ellipsoid paramater:               %varargin{ 3 4 5 6} should be: scale of a b c, and di
tk = Tracker( 'ellipsoid' , capturer , 1.2 , 0.5 , 0.5 , 0.7 );
tk.showBoundary();
showSupportSpace( obj , tk , 1 );
%

%cylinder paramater:                %varargin{ 3 4 } should be : scale of a, di
tk = Tracker( 'cylinder' , capturer , 1.2 , 10 );
tk.showBoundary();
showSupportSpace( obj , tk , 1);
%


