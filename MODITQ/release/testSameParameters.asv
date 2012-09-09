function [] = testSameParameters( XX , Xtraining , WtrueTestTraining , D , num_training , bit , method )

%several state of art methods
switch(method)
    
    % ITQ method proposed in our CVPR11 paper
    case 'ITQ'
        % PCA
        [pc, l] = eigs(cov(XX(1:num_training,:)),bit);
        XX = XX * pc;
        % ITQ
        [Y, R] = ITQ(XX(1:num_training,:),50);
        XX = XX*R;
        Y = zeros(size(XX));
        Y(XX>=0) = 1;
        Y = compactbit(Y>0);
    % RR method proposed in our CVPR11 paper
    case 'RR'
        % PCA
        [pc, l] = eigs(cov(XX(1:num_training,:)), bit);
        XX = XX * pc;
        % RR
        R = randn(size(XX,2),bit);
        [U S V] = svd(R);
        XX = XX*U(:,1:bit);
        Y = compactbit(XX>0);
   % SKLSH
   % M. Raginsky, S. Lazebnik. Locality Sensitive Binary Codes from
   % Shift-Invariant Kernels. NIPS 2009.
    case 'SKLSH' 
        RFparam.gamma = 1;
        RFparam.D = D;
        RFparam.M = bit;
        RFparam = RF_train(RFparam);
        B1 = RF_compress(XX(1:num_training,:), RFparam);
        B2 = RF_compress(XX(num_training+1:end,:), RFparam);
        Y = [B1;B2];
    % Locality sensitive hashing (LSH)
     case 'LSH'
        XX = XX * randn(size(XX,2),bit);
        Y = zeros(size(XX));
        Y(XX>=0)=1;
        Y = compactbit(Y);
        
    % our own novel 1 first itq, than find a good s to improve the
    % sensitivity
    case 'ITQS'
        % PCA
        [pc, l] = eigs(cov(XX(1:num_training,:)),bit);
        %XX = XX * pc;
        % ITQ
        [Y, R] = ITQ(XX(1:num_training,:) * pc ,50 );
        %XX = XX*R;
        %Y = zeros(size(XX));
        %Y(XX>=0) = 1;                        %should We prevent this ?
        q = 10;                               %q is fro [-q,q] user define boundary
        Q = eye( size( pc,1 ) );              %q is diag dxd matrix
        Q = ( q^2 / 3 ) * Q ;
        A1 = pc' * (XX(1:num_training,:))' * XX(1:num_training,:) * pc;
        A2 = pc' * Q * pc ;
        S = ( A1 + A1' + A2 + A2' ) \ ( R * Y' * XX(1:num_training,:) * pc )';  %omitting (RR')-1 ||  %not to use inv?? || RR' must be eye right?
        
        XX = XX * pc ;
        XX = XX*R;
        XX = XX*S;
        
        Y = zeros(size(XX));
        Y(XX>=0) = 1;
        
        Y = compactbit(Y>0);
end

% compute Hamming metric and compute recall precision
B1 = Y(1:size(Xtraining,1),:);
B2 = Y(size(Xtraining,1)+1:end,:);
Dhamm = hammingDist(B2, B1);
[recall, precision, rate] = recall_precision(WtrueTestTraining, Dhamm);

% plot the curve
switch(method)
    % ITQ method proposed in our CVPR11 paper
    case 'ITQ'
    plot(recall,precision,'-o');
    case 'RR'
    plot(recall,precision,'-s');
    case 'SKLSH' 
    plot(recall,precision,'-d');
     case 'LSH'
    plot(recall,precision,'-<');
    case 'ITQS'
    plot(recall,precision,'-X');    
    case 'ITQI'
    plot(recall,precision,'-*');            
end

xlabel('Recall');
ylabel('Precision');

end