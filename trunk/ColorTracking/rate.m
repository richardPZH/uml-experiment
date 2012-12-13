function [] = rate( )

% test the video output 
% frame = getsnapshot()
% imshow();
% will this two slow? If it is, what shall we do? Use c/c++ instead?
%
% After effect: it seems 320x240 rgb still ok

info = imaqhwinfo

winfo = imaqhwinfo('winvideo')

obj = videoinput( 'winvideo' , 1 , 'YUY2_320x240' );

himage = preview( obj );
set( obj , 'ReturnedColorSpace' , 'rgb' );

figure( 2 );

while ( 0 )
    frame = getsnapshot( obj );
    
    imshow( frame );
end