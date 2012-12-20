function [] = MyDraw() 

% The main m file of this project
info = imaqhwinfo
winfo = imaqhwinfo('winvideo')
obj = videoinput( 'winvideo' , 1 , 'YUY2_640x480' ) ; %'YUY2_320x240' );
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

alphaArea = 2;   %this is ¦Á, the threshold( the min support area ) defined the LOSS of target, vital!
winWidth = 5;     %this is the width of the target white border width, vital!


TrackingWindow = [ 128 , 128 ];
SubSample = [ 10 , 10 ];

stepR = floor( SubSample(1) / 2 );
stepC = floor( SubSample(2) / 2 );

m = floor( TrackingWindow( 1 ) / 2 );
n = floor( TrackingWindow( 2 ) / 2 );
figure(3);
title('Tracking Window');

frame = getsnapshot( obj );
[ maxR maxC maxColor ] = size( frame );
maskTrace = zeros( maxR , maxC , 'uint8' );


X = zeros( ( m + m + 1 ) * ( n + n + 1 ) , 1 );
Y = zeros( ( m + m + 1 ) * ( n + n + 1 ) , 1);
index = 1;
tmp = zeros( 1 , 1 , 3 );

%The tracking window center is ( xrow , ycol )
xrow = tk.Location(1);
ycol = tk.Location(2);
speed = tk.speed;

%Pre allocate the space

border = 255 * ones( 5 , 5 , 'uint8' );

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
    
    if index < alphaArea
        %This is LOST, we need to recover
        tk = Recover( tk , obj , alphaArea , [ 10 , 10 ] );
        xrow = tk.Location(1);
        ycol = tk.Location(2);
        speed = tk.speed;
        maskTrace = zeros( maxR , maxC , 'uint8' );
    else
        index = index - 1;
        %Find new Center 
        ncenterR = floor( sum( X(1:index)) / index ) ; %floor is better that ceiling
        ncenterC = floor( sum( Y(1:index) ) / index );
        speed = [ ncenterR - xrow , ncenterC - ycol ];
    
        xrow = ncenterR;
        ycol = ncenterC;
        tk.Location = [ xrow , ycol ];
    
        %Add the mask:
        if ~ ( ( xrow -2 < 1 ) || ( xrow + 2 > maxR ) || ( ycol - 2 < 1 ) || ( ycol + 2 > maxC ) )
            maskTrace( xrow - 2 : xrow + 2 , ycol - 2 : ycol + 2 ) = maskTrace( xrow - 2 : xrow + 2 , ycol - 2 : ycol + 2 ) + border;
        end
        
        frame(:,:,1) = frame( : , : , 1 ) + maskTrace;
        frame(:,:,2) = frame( : , : , 2 ) + maskTrace;
        frame(:,:,3) = frame( : , : , 3 ) + maskTrace;
        
%       frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 2 ) ...
%         = frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 2 ) + border;
%     
%     frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 3 ) ...
%         = frame( xrow - radius - winWidth : xrow + radius + winWidth - 1 , ycol - radius - winWidth : ycol + radius + winWidth - 1 , 3 ) + border;
    end
    
    hdl = figure( 3 );
    imshow( frame );
    %Get anotehr new frame and update variables, vital
    frame = getsnapshot( obj );
    index = 1;
    
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
