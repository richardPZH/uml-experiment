function [ E1 ] = getEntrance1( trGist , trlabels , bit , method )
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
%     E1 , the Entrance table for Level1 , is a cell( n , 3 );
%          the E1( x , 1 ) stores W0 R0 cP0
%          the E1( x , 2 ) stores the binary code matrix
%          the E1( x , 3 ) stores a cell contain indice 


[W0 R0 cP0] = Level1Hash2( trGist , trlabels , bit , method );

%need to Calculate the J(R,1) and store the buckets This will use to
%evaluate good or bad of a [ a b c ]

XX =  trGist - repmat( cP0 , size( trGist , 1 ) , 1 );
XX = XX * W0 * R0;
XX( XX >= 0 ) = 1;
XX( XX <  0 ) = 0;

[ b i j ] = unique( XX , 'rows' );

E1 = cell( 1 , 3 );

% store the [W0 R0 cP0] in E1(1,1)
E1{ 1 , 1 } = { W0 ,  R0 , cP0 };

% store the L1Code matrix in E1(1,2)
E1{ 1 , 2 } = b;
anoymous = cell( size( b , 1 ) , 1 );

for m = 1 : size( b , 1 )
	anoymous{ m } = find( j == m );
end

% store the indice of the trGist in E1(1,3)
E1{ 1 , 3 } = anoymous;


