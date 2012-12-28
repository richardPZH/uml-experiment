function [ ] = hughMain( )

obj = videoinput( 'winvideo' , 1 , 'YUY2_320x240' );
set( obj , 'ReturnedColorSpace' , 'rgb' );

preview( obj );

while( 1 )
    frame = getsnapshot( obj );
    
    g2 = rgb2gray( frame );
    
    Bw = edge( g2 , 'canny' );
    
    imshow( Bw );
    
end



% frame = getsnapshot( obj );
% 
% gray2 = rgb2gray( frame );
% 
% cannyEdge = edge( gray2 , 'canny' );
% 
% [hough_space,hough_circle,para] = hughCircle( cannyEdge , 20 , pi , 50 , 80 , 0.5 );
 
