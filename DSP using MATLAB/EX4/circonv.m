function y = circonv(x1,x2)
% Develops a sequence y obtained by
% circularly convolving two finite-length
% sequences x1 and x2.
% x1,x2 must have identical lengths.
L1 = length(x1); L2 = length(x2);
if L1 ~= L2 % vectors must be of equal lenghts to circularly convolve.
   error('Sequences of unequal lengths') % throws exception and returns.
end
y = zeros(1,L1); % output initializer.
x2tr = [x2(1) x2(L2:-1:2)]; % flip x2 cyclicly.
for k = 1:L1 % repeat L1 times
	sh = circshift(x2tr,1-k); % sh = cyclic shift by 1-k times.
	h = x1.*sh;% element-wise multiply flipped and shifted signal x2 by x1.
	y(k) = sum(h);% assign the convolution value at k as sum of element-wise
                    % multiplication.
end
