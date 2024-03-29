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
capturer = getCapturer( obj , 'getpts' );

%This read the same data
%
% load 'green.mat'     % get data , a mx3 [ r g b ] vector
%         
% capturer.location = [ 128 , 128 ];  %give this initial location is row=128,col=128
% capturer.data = data;
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


% %ellipsoid paramater:               %varargin{ 3 4 5 6} should be: scale of a b c, and di
% tk = Tracker( 'ellipsoid' , capturer , 1.3 , 0.7 , 0.7 , 0.7 );
% tk.showBoundary();
% %showSupportSpace( frame , tk , 1 );
% while( 1 )
%     frame = getsnapshot( obj );
%     showSupportSpace( frame , tk , 7 );
% end
% %
% %

%cylinder paramater:                %varargin{ 3 4 } should be : scale of a, di
% tk = Tracker( 'cylinder' , capturer , 1.2 , 9 );
% tk.showBoundary();
% %showSupportSpace( frame , tk , 1 );
% while( 1 )
%     frame = getsnapshot( obj );
%     showSupportSpace( frame , tk , 7 );
% end

% This test them all !!!
hdl = figure( 4 );

tk_sphere    = Tracker( 'sphere' , capturer , 0.7 ); 
tk_ellipsoid = Tracker( 'ellipsoid' , capturer , 1.3 , 0.68 , 0.68 , 0.68 );
tk_cylinder  = Tracker( 'cylinder' , capturer , 1.3 , 9.5 );

%we make a smart planet
% while( 1 )
%     frame = getsnapshot( obj );
%     
%     showSupportSpace( hdl , 1 ,  frame , tk_sphere  , 2 );
%     
%     showSupportSpace( hdl , 2 , frame , tk_ellipsoid , 2 );
%     
%     showSupportSpace( hdl , 3 , frame , tk_cylinder , 2 );
%     
%     %we use the ginput( 1 ) to premote use to continue
%     [ X , Y ] = ginput( 1 );
%     if X < 1 || X > size( frame , 2 ) ||  Y < 1 || Y > size( frame , 1 )
%         error('End Of Test');
%     end
%     %
%     
% end


while( 1 )
    frame = getsnapshot( obj );
    
    showSupportSpace( hdl , 1 ,  frame , tk_sphere  , 4 );
    
    showSupportSpace( hdl , 2 , frame , tk_ellipsoid , 4 );
    
    showSupportSpace( hdl , 3 , frame , tk_cylinder , 4 );
    
end



