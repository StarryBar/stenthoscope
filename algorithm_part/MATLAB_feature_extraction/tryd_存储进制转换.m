I = importdata('E:\心脏或大血管间异常通道.dat');
[M,N] = size(I);
d = zeros(M,N);
for i = 1:M  
    s = cell2mat(I(i));
    d(i) = hex2dec(s(3:end));
    if (d(i) > 32767)
        d(i) = d(i) - 65536;
    end
end
d=int8(d./max(abs(d)).*100);
figure1 = figure('Color',[1 1 1]);
plot(d);
%DLMWRITE('E:\matb\心脏或大血管间异常通道.txt',d,'delimiter','\t','precision',6);