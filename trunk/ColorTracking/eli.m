function [X Y Z] = eli( a , b , c )

% we plot the x^2/a^2 + y^2/b^2 + z^2/c2 =1 
%

X = 0;
Y = 0;
Z = 0;

a2 = a*a;
b2 = b*b;
c2 = c*c;

for x = -a : 0.1 : a
    for y = -b : 0.1 : b
        tmp = 1 - x*x / a2 - y*y / b2;
        
        if( tmp > 0 )
           z = sqrt( tmp * c2 );
           
           X = [ X x x];
           Y = [ Y y y];
           Z = [ Z z -1*z];
                      
        end
        
    end
end