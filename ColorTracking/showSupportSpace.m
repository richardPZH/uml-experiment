function [] = showSupportSpace( obj ,  tk ) 

% The showSupportSpace is used to evaluate the different tk performance

frame = getsnapshot( obj );

[row col] = size( frame );

nframe = zeros( row , col , 'uint8' );

step = 8;

for m = 1 : step : row
    for n = 1 : step : col
        tmp = frame( m , n , : );
        
        x = [ tmp(1) ; tmp(2); tmp(3) ];
        
        nframe = uint8( 255 * tk.isTargetColor( x ) );     
                
    end
end

figure;
imshow( nframe );
