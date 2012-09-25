function [ num ] = t( kong )

if kong == 1 || kong == 0
    num = 0;
else
    num = t( ceil( kong / 2 ) ) + floor( kong/2  ) ;
end
