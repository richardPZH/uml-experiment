function [] = showSupportSpace( hdl , SubPlotNum , frame , tk , step ) 

% The showSupportSpace is used to evaluate the different tk performance
% we use a persistent variable to act like a static in c, useful to test

persistent walker;       %declare a static-like variable walker
if isempty( walker )     %if walker is not assigned yet, we initial it
    walker = 0;
else
    walker = walker + 1;
end

turn = floor( walker / 3 );
if turn == step * step 
    turn = 1;
    walker = 0;
end

mStart = floor( turn / step + 1 );
nStart = floor( mod( turn ,step ) + 1 );

% default argument step
if nargin < 3
    step = 8;
end

[row col color] = size( frame ); %always remember a RGB image is mxnxl

nframe = zeros( row , col , 'uint8' );

for m = mStart : step : row
    for n = nStart : step : col
        tmp = frame( m , n , : );
        
        x = [ tmp(1) ; tmp(2); tmp(3) ];
        
        nframe( m , n ) = uint8( 255 * tk.isTargetColor( x ) );     
                
    end
end

figure( hdl );
subplot( 1 , 3 , SubPlotNum );

imshow( nframe );


switch tk.tkType
    case 'sphere'
        title('Sphere');
    case 'ellipsoid'
        title('Ellipsoid');        
    case 'cylinder'
        title('Cylinder');        
end

