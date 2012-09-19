function [B,R] = procrustean( X , W , Labels , n_iter )
%
% main function for procrustean which finds a rotation of the CCA projected data
%
% Input:
%       X: the original data, n*d
%       W: d*c project matrix, CCA project matrix     
%       Labels: n*1 a label matrix {1~n} , can be clean or noisy
%       n_iter: max number of iterations, 50 is usually enough
% Output:
%       B: n*c binary matrix
%       R: the c*c rotation matrix found by procrustean
%       S: the c*c diagnal matrix used fro dragging
% Author:
%       IMS@SCUT

% We first get the CCA projected data V;
V = X * W;

% initialize with a orthogonal random rotation
bit = size(V,2);
R = randn(bit,bit);
[U11 S2 V2] = svd(R);
R = U11(:,1:bit);

%used to plot the correct time to stop
J = zeros( 1 , n_iter );
Q = diag( sqrt( var( X )));


weight = ( size( V , 2 )-1) : -1 : 0 ;
weight = 2.^weight;
weight = weight';
% use ITQ to find optimal rotation, but change the UX according to their
% label
for iter=1:n_iter
    Z = V * R;      
    UX = ones(size(Z,1),size(Z,2)).*-1;
    UX(Z>=0) = 1;                         
    %Now the UX is the new B , we need to change it!
    for i = 1 : ( max( Labels ) + 1 )
        index =  find( Labels == ( i-1 ) );   %handle this class label, labels is from 0 ~ t
        UI = UX( index , : );
        UI( UI<0 ) = 0;
        num = UI * weight;
        [ a b ] = mode( num );
        num_i = find( num == a );
        UX( index , : ) = repmat( UX( num_i(1) , : ) , length( index ) , 1 );
    end
    
    J(iter) = trace( 1/4 * (UX' * UX) ) - trace( UX' * X * W * R ) + trace( R' * W' * ( X' * X ) * W * R ) +  trace( R' * W' * Q * W * R );
   
    D = V' * UX;
    G = sqrtm( D * D' );
    R = pinv( G ) * D;       % change inv(D) * G into inv(G)*D    
    
end

% make B binary
B = UX;
B(B<0) = 0;

figure( 2 );
x = 1 : n_iter ;
plot( x , J , 'r.:' );










