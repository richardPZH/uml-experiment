function [] = testDiffModel() 

% The file test different ability of modules
% Author:
%       IMS@SCUT Once
%       WMDZTSXCDH

info = imaqhwinfo
winfo = imaqhwinfo('winvideo')
obj = videoinput( 'winvideo' , 1 ,  'YUY2_320x240' ); %'YUY2_640x480' ); %'RGB24_320x240' );
himage = preview( obj );
video = figure(1);
set( obj , 'ReturnedColorSpace' , 'rgb' );

%getCapturer paramaters:           %'default_red' | 'getpts' | 'getrect'
%capturer = getCapturer( obj , 'getpts' );

%This read the same data
%
load 'green.mat'     % get data , a mx3 [ r g b ] vector
        
capturer.location = [ 128 , 128 ];  %give this initial location is row=128,col=128
capturer.data = data;
%
%end reading data
frame = getsnapshot( obj );

% %sphere paramater:
%                                   %varargin{ 3 } may be : 'a' 'b' 'c' 'avg' preOfavg 
% tk = Tracker( 'sphere' , capturer , 0.6 ); 
% tk.showBoundary();
% %showSupportSpace( frame , tk , 1 );  % third 1 is the step
% %We show how the machine see the world
% while( 1 )
%     frame = getsnapshot( obj );
%     showSupportSpace( frame , tk , 7 );
% end


%ellipsoid paramater:               %varargin{ 3 4 5 6} should be: scale of a b c, and di
tk = Tracker( 'ellipsoid' , capturer , 1.3 , 0.7 , 0.7 , 0.7 );
tk.showBoundary();
%showSupportSpace( frame , tk , 1 );
while( 1 )
    frame = getsnapshot( obj );
    showSupportSpace( frame , tk , 7 );
end
%
%

%cylinder paramater:                %varargin{ 3 4 } should be : scale of a, di
% tk = Tracker( 'cylinder' , capturer , 1.2 , 9 );
% tk.showBoundary();
% %showSupportSpace( frame , tk , 1 );
% while( 1 )
%     frame = getsnapshot( obj );
%     showSupportSpace( frame , tk , 7 );
% end
%


