function [avg_precision] = testMAP(X , method )
%
%

[recall, precision16] = test(X, 16, method);
[recall, precision32] = test(X, 32, method);
[recall, precision64] = test(X, 64, method);
[recall, precision128] = test(X, 128, method);
[recall, precision256] = test(X, 256, method);

avg_precision = [ mean( precision16 ) mean(precision32)  mean( precision64 ) mean( precision128 ) mean( precision256 )];

x = ( 4 : 8 );
x = 2.^x;
switch(method)
    
    % ITQ method proposed in our CVPR11 paper
    case 'ITQ'
    plot( x , avg_precision , '-o' );
    % RR method proposed in our CVPR11 paper
    case 'RR'
    plot( x , avg_precision , '-s');
   % SKLSH
   % M. Raginsky, S. Lazebnik. Locality Sensitive Binary Codes from
   % Shift-Invariant Kernels. NIPS 2009.
    case 'SKLSH' 
    plot( x , avg_precision , '-d' )  
    % Locality sensitive hashing (LSH)
     case 'LSH'
    plot( x , avg_precision , '-<' )      
    % our own novel 1 first itq, than find a good s to improve the
    % sensitivity
    case 'ITQS'
    plot( x , avg_precision , '-X' )
end

xlabel('Number of bits');
ylabel('mAP');




