function [ radius ] = findRadius( center , X , Y , method )

%find the radius of the new found object
%
%

data = [ X , Y ];

cha = data - repmat( center , size( data , 1 ) , 1 );
cha = sqrt( sum( cha.^2 , 2 ) );

switch method 
    case 'max'
        radius = max( cha );
    case 'mean'
        radius = mean( cha );
    case 'median'
        radius = median( cha );
    case 'hyper'
        radius = median( cha ) + 0.25 * max( cha );
end
