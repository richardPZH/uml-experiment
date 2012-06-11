function [ ] = discreteTheSampleSet()

load yeast.out

[ DiscretData , DiscretizationSet1 ] = CACC_Discretization( yeast , 1 );

fid = fopen('yeast.dis','w');
fprintf( fid,'%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n', DiscretData' );
fclose(fid);


end