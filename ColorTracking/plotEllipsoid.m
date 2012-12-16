function [] = plotEllipsoid( numClick , windowSize , thick )

% test the assumption of the Tracking Objects By Color Alone
% plot the ellipsoid 
% default get click is 5

if nargin < 1
    numClick = 5;
    windowSize = [14 14];
    thick = 8;
elseif nargin <2
    windowSize = [14 14];
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
Z = floor( X );
X = floor( Y );
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
axis( [ 0 255 0 255 0 255 ] );
grid on;
xlabel('red');
ylabel('green');
zlabel('blue');

%start to find the ellipsoid in arbitrary orention
data = [ r' , g' , b' ];

cp = mean( data );
cp = cp';

cm = cov( data ); %find the covariance matrix

[ P , Lam ] = eigs( cm ); %eigenvector eigenvalue

poj = data * P(:,1);
a = max( poj ) - min( poj );
a = a / 2;

poj = data * P(:,2);
b = max( poj ) - min( poj );
b = b / 2;

poj = data * P(:,3);
c = max( poj ) - min( poj );
c = c / 2;

a2 = a * a;
b2 = b * b;
c2 = c * c;

B = diag( [ a2 b2 c2 ] );

A = P * B * inv( P );

rvA = inv( A );   % ( x - cp )' * ivA * ( x - cp ) = 1 or > 1 or < 1

%We need modify the a2 b2 c2 to to change!
threshold = 0.5;
showMyEllipsoid( cp , rvA , threshold );

figure( 4 );

while( 1 )
   frame = getsnapshot( obj );
   [r c color] = size( frame );
   nframe = zeros( r , c , 'uint8' );
   
   for row = 1 : 8 : r
       for col = 1 : 8 : c
           red = frame( row , col , 1 );
           green = frame( row , col , 2 );
           blue = frame( row , col , 3 );
           
           x = [ red green blue ]';
           
           if( ( double(x) - cp )' * rvA * ( double(x) - cp ) < threshold )
               nframe( row , col ) = 255;
           end
           
       end
   end
   
   imshow( nframe );
   
end












