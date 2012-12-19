function [] = showSupportSpace( frame , tk , step ) 

% The showSupportSpace is used to evaluate the different tk performance

% default argument step
if nargin < 3
    step = 8;
end

[row col color] = size( frame ); %always remember a RGB image is mxnxl

nframe = zeros( row , col , 'uint8' );

for m = 1 : step : row
    for n = 1 : step : col
        tmp = frame( m , n , : );
        
        x = [ tmp(1) ; tmp(2); tmp(3) ];
        
        nframe( m , n ) = uint8( 255 * tk.isTargetColor( x ) );     
                
    end
end

figure(2);
imshow( nframe );
