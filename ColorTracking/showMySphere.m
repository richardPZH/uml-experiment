function [] = showMySphere( tk , range )

% show the Sphere that I find
% In the RGB space
% range is 0-255 default

if nargin < 2
    range = [ 0 255 ];
end

v = tk.CenterOfSphere;
r = tk.Radius;
r2 = r * r;

step = 8;
mi = range( 1 );
ma = range( 2 );

X = 0;
Y = 0;
Z = 0;

for x = mi : step : ma
    for y = mi : step : ma
        for z = mi : step : ma 
            
            rgb = [x y z]';
            
            if( ( sum( rgb.^2 + v.^2 ) - 2 * rgb' * v ) < r2 )
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

figure;
plot3( X , Y , Z , 'o' , 'markerfacecolor' , 'b' );
title('RGB-Sphere, Within Sphere is Accepted');
grid on;
axis( [0 255 0 255 0 255 ] );
xlabel('reg');
ylabel('green');
zlabel('blue');





