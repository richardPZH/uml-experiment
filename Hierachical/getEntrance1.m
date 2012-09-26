function [W0 R0 cP0 E1 ] = getEntrance1( trGist , trlabels , bit , method )
% 
% This function performs the first level hashing, its purpose is to roughtly group 
% the general classes together.  e.g. 
%     living thing vs. nonliving thing;
%     things on land / on ocean / in the air
%
% Input:
%     trGist, the nx320 CIFAR , Gist represented images as a learning samples
% 	  trlabels, the nx1 CIFAR labels, 0~9
%     bit, how many bit is assigned to this hash code
%     method, e.g 'ITQ' 'OURSITQ' 'ITQRR'
%
% Output:
%     W0 , the projection matrix found using the CCA
%     R0 , the orthogonal matrix found by ITQ like algorithm
%     cP0 , centerPoint of the training samples , usefully when searching
%     E1 , the Entrance table for Level1 , read your design!!


[W0 R0 cP0] = Level1Hash2( trGist , trlabels , bit , method );

%need to Calculate the J(R,1) and store the buckets

XX = trGist - repmat( centerPoint0 , size( trGist , 1 ) , 1 );
XX = XX * W0 * R0;
XX( XX >= 0 ) = 1;
XX( XX <  0 ) = 0;

[ b i j ] = unique( XX , 'rows' );

E1 = cell( size( b , 1 ) , 2 );

for m = 1 : size( b , 1 )
	E1{ m , 1 } = b( m , :);
	E1{ m , 2 } = find( j == m );
end
