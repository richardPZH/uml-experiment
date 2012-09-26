function [W0 R0 centerPoint0 Entrance1 ] = getEntrance1( trGist , trlabels , firstBit , method )
% 
%  
%
[W0 R0 centerPoint0] = Level1Hash2( trGist , trlabels , firstBit , method );

%need to Calculate the J(R,1) and store the buckets
XX = trGist - repmat( centerPoint0 , size( trGist , 1 ) , 1 );
XX = XX * W0 * R0;
XX( XX >= 0 ) = 1;
XX( XX <  0 ) = 0;

[ b i j ] = unique( XX , 'rows' );
Entrance1 = cell( size( b , 1 ) , 2 );

for m = 1 : size( b , 1 )
	Entrance1{ m , 1 } = b( m , :);
	Entrance1{ m , 2 } = find( j == m );
end