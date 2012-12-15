function [] = plotEllipsoid( numClick , windowSize , thick )

% test the assumption of the Tracking Objects By Color Alone
% plot the ellipsoid 
% default get click is 5

if nargin < 1
    numClick = 5;
    windowSize = [16 16];
    thick = 8;
elseif nargin <2
    windowSize = [16 16];
    thick = 8;
elseif nargin < 3
    thick = 8;
end

info = imaqhwinfo
winfo = imaqhwinfo('winvideo')
obj = videoinput( 'winvideo' , 1 , 'YUY2_320x240' );
himage = preview( obj );
video = figure(1);
set( obj , 'ReturnedColorSpace' , 'rgb' );

hdl = figure( 2 );
frame = getsnapshot( obj );
nframe = frame;
imshow( frame );

m = floor( windowSize(1) / 2 );
n = floor( windowSize(2) / 2 );

%rect = getrect( hdl );
title('Press Enter to End Selecting Points');
% Need the switch X and Y
[X,Y] = getpts( hdl );
Z = X;
X = Y;
Y = Z;
clear Z; 
%[X,Y] = ginput( 1 );
    
%no subsample of the selected windows, we may subsample too!
index = 1;
r = zeros( 1 , (2*m+1) * (2*n+1) * length(X) );
g = r;
b = r;
l = ( 2*m + 1 ) * ( 2*n + 1 ) - 1;

for a = 1 : length( X )
   tmp = frame( ( X(a) - m ) :( X(a) + m ) , ( Y(a) - n ) : ( Y(a) + n ) , : );
   r( index : index + l ) = reshape( tmp( : , : , 1 ) , 1 , [] );
   g( index : index + l ) = reshape( tmp( : , : , 2 ) , 1 , [] );
   b( index : index + l ) = reshape( tmp( : , : , 3 ) , 1 , [] );
   
   index = index + l + 1;
end

rgbHdl = figure( 3 );
plot3( r , g , b , 'o' , 'markerfacecolor' , 'b' );
axis( [ 0 300 0 300 0 300 ] );
grid on;
xlabel('red');
ylabel('green');
zlabel('blue');








