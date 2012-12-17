function [] = main() 

% The main m file of this project
info = imaqhwinfo
winfo = imaqhwinfo('winvideo')
obj = videoinput( 'winvideo' , 1 , 'YUY2_320x240' );
himage = preview( obj );
video = figure(1);
set( obj , 'ReturnedColorSpace' , 'rgb' );

%getCapturer paramaters:           %'default_red' | 'getpts' | 'getrect'
capturer = getCapturer( obj , 'getpts' );

%sphere paramater:
%                                   %varargin{ 3 } may be : 'a' 'b' 'c' 'avg' preOfavg 
%tk = Tracker( 'sphere' , capturer , 'avg' ); 
%

%ellipsoid paramater:               %varargin{ 3 4 5 6} should be: scale of a b c, and di
tk = Tracker( 'ellipsoid' , capturer , 1 , 0.6 , 0.6 , 0.7 );
%

%cylinder paramater:                %varargin{ 3 4 } should be : scale of a, di
%tk = Tracker( 'cylinder' , capturer , 1 , 10 );
%

%tk.showBoundary();

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%strat tracking no IFA is in used! 
%We can add a mask to get a input toy
%mask = zeros( );
%radius = 5;
%color = white

alpha = 30;  %this is ¦Á, the threshold defined the LOSS of target, vital!
winWidth = 5; %this is the width of the target white border width, vital!

TrackingWindow = [ 100 , 100 ];
SubSample = [ 8 , 8 ];

stepR = floor( SubSample(1) / 2 );
stepC = floor( SubSample(2) / 2 );

m = floor( TrackingWindow( 1 ) / 2 );
n = floor( TrackingWindow( 2 ) / 2 );
figure(2);
title('Tracking Window');

frame = getsnapshot( obj );
[ maxR maxC maxColor ] = size( frame );

X = zeros( ( m + m + 1 ) * ( n + n + 1 ) , 1 );
Y = zeros( ( m + m + 1 ) * ( n + n + 1 ) , 1);
index = 1;
tmp = zeros( 1 , 1 , 3 );

%The tracking window center is ( xrow , ycol )
xrow = tk.Location(1);
ycol = tk.Location(2);
speed = tk.speed;

%Pre allocate the space
ncenterR = 1;
ncenterC = 1;

srow = 1;
erow = 1;
scol = 1;
ecol = 1;

while( 1 ) %endless loop
    
    %showSupportSpace( obj , tk , 16 );
      
    xrow = xrow + speed(1); %mod ( xrow + speed(1) , maxR );
    ycol = ycol + speed(2); %mod ( ycol + speed(2) , maxC );
    
    %Prepare the Tracking window, start row, end row, start col, end col
    srow = xrow - m;
    erow = xrow + m;
    scol = ycol - n;
    ecol = ycol + m;
    
    if srow < 1
        srow = 1;
    end
    if erow > maxR
        erow = maxR;
    end
    if scol < 1
        scol = 1;
    end
    if ecol > maxC
        ecol = maxC;
    end
    
    for rn = srow : stepR : erow
        for rc = scol : stepC : ecol
            tmp = frame( rn , rc , : );
            x = [ tmp(1) ; tmp(2) ; tmp(3) ];
            
            if isTargetColor( tk , x )       %Maybe this is faster isTargetColor( tk , x )
                X(index) = rn;
                Y(index) = rc;
                index = index + 1;
            end
        end
    end
    
    if index < alpha
        %This is LOST, we need to recover
        continue;
    end
    
    index = index - 1;
    %Find new Center 
    ncenterR = floor( sum( X(1:index)) / index ) ; %floor is better that ceiling
    ncenterC = floor( sum( Y(1:index) ) / index );
    speed = [ ncenterR - xrow , ncenterC - ycol ];
    
    xrow = ncenterR;
    ycol = ncenterC;
    
    %the area to fill, we need to save time
    %assume all the support point is average distribute in a circle, find radius
    radius = floor( sqrt( length( index ) / 4 ) );
    border = zeros( 2*radius + 2*winWidth , 'uint8' );
    border( 1:winWidth , : ) = 255;
    border( end-winWidth+1 : end , : ) = 255;
    border( : , 1:winWidth ) = 255;
    border( : , end-winWidth+1 : end ) = 255;
    
    %This can be wrong!!!
    frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 1 ) ...
        = frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 1 ) + border;
    
%     frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 2 ) ...
%         = frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 2 ) + border;
%     
%     frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 3 ) ...
%         = frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 3 ) + border;

    hdl = figure( 3 );
    imshow( frame );
    %Get anotehr new frame and update variables, vital
    frame = getsnapshot( obj );
    index = 1;
    
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
