function [ Distance ] = CalHammingDist( sample , codes , method )
% This function calculate the distance of sample against the codes
% sample and codes are both double vectors
%
% Input:
%     sample , the 1xd row vector; 
%     codes, the nxd matrix;
%     method, the way to calculate the Distance, to test the efficiency
%         'for', 'vec' is supported
%
% Output:
%     Distance, nx1 column vector, each Distance(i) = dist( sample , codes(i,:);
%
% Authors:
%     IMS@SCUT Once
%     2012/09/25
%


d0 = size( sample , 2 );
[ n d ] = size( codes );

if d0 ~= d
	disp( 'CalHammingDist: Warning code length is different!!' );
	return;
end


switch method

case 'for'
	Distance = zeros( n , 1 );
	for i = 1:n
		dist = 0;

		for j = 1:d
			dist = dist + ( sample( j ) ~= codes( i , j ) );
		end

		Distance( i )= dist;
	end

case 'vec'
	
	Distance = codes - repmat( sample , n , 1 );
	Distance( Distance ~= 0 ) = 1;
	Distance = sum( Distance , 2 );

end



