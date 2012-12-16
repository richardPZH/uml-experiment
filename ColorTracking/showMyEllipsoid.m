function [] = showMyEllipsoid( v , ivA , d , range )

% show the Ellipsoid that I find
% In the RGB space
% range is 0-255 default

if nargin < 3
    d = 1;
    range = [ 0 255 ];
elseif nargin < 4
    range = [ 0 255 ];
end

step = 16;
mi = range( 1 );
ma = range( 2 );

X = 0;
Y = 0;
Z = 0;

for x = mi : 8 : ma
    for y = mi : 8 : ma
        for z = mi : 8 : ma 
            
            rgb = [x y z]';
            
            if( (rgb - v)' * ivA * (rgb -v ) < d )
                X = [X x ];
                Y = [Y y ];
                Z = [Z z ];
            end
            
        end
    end
end

X(1) = [];
Y(1) = [];
Z(1) = [];

figure( 5 );
plot3( X , Y , Z , 'o' , 'markerfacecolor' , 'b' );
grid on;
axis( [0 255 0 255 0 255 ] );





