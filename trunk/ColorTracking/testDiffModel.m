function [] = testDiffModel() 

% The main m file of this project
info = imaqhwinfo
winfo = imaqhwinfo('winvideo')
obj = videoinput( 'winvideo' , 1 ,  'RGB24_320x240' ); %'YUY2_320x240' );
himage = preview( obj );
video = figure(1);
set( obj , 'ReturnedColorSpace' , 'rgb' );

%getCapturer paramaters:           %'default_red' | 'getpts' | 'getrect'
capturer = getCapturer( obj , 'getpts' );

%sphere paramater:
%                                   %varargin{ 3 } may be : 'a' 'b' 'c' 'avg' preOfavg 
tk = Tracker( 'sphere' , capturer , 'avg' ); 

%

%ellipsoid paramater:               %varargin{ 3 4 5 6} should be: scale of a b c, and di
%tk = Tracker( 'ellipsoid' , capturer , 2 , 0.5 , 0.5 , 1 );
%

%cylinder paramater:                %varargin{ 3 4 } should be : scale of a, di
tk = Tracker( 'cylinder' , capturer , 1 , 10 );
%

tk.showBoundary();
