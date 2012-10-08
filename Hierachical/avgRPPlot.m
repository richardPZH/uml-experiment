function [ r p ] = avgRPPlot( recall , precision , step )

% fuction avgRPPlot
% This function will rearrange the recall , precision and plot them in a curve
% Since the recall, precision contains multiple line of recall-precision curves
% We need to rearrange them and calculate the average recall-precision curve
%
% This funcion is very important, it is used for evaluate a system's performance
% Am I doing the right thing?
% 
% Input:
%     recall , the 1xn range from 0~1, the recall may be disordered
%     precision, the 1xn range from 0~1, correspond precision
%	  step, the step calculate the avg recall, usually 0.02 - 0.1; 0.05?
%
%  
% Output:
%     r -> the averaged recall it is also ordered
%     p -> correspond precision
% 
% Author:
%     IMS@SCUT Once 2012/10/08
%

if step <= 0 || step >= 1
    disp('Warning: function avgRPPlot receive bad argument: step' );
    return;
end


[ recall , index ] = sort( recall );  %check this out, ascend and index
precision = precision( index );       %the precision is rearrange too


r = zeros( 1 , ( 1 / step + 1 ) );            %Maybe it has little bug
p = zeros( size(r) );                         %It's a bigger container

%How to vectorization my programs?

m = 1;
n = 1;
l = length( recall );

while( m <= l )

	upper =( n - 0.5 ) * step;
	ps = 0;
	counter = 0;

	while( ( m <= l ) && ( recall( m ) <= upper ) )

		ps = ps + precision( m );
		m = m + 1;
		counter = counter + 1;

	end

	r( n ) = ( n - 1 ) * step ;
	p( n ) = ps / ( counter + eps );

	n = n + 1;

end

r( n : end ) = [];
p( n : end ) = [];





