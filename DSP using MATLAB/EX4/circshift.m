function y = circshift(x,M)
% Develops a sequence y obtained by
% circularly shifting a finite-length
% sequence x by M samples
if abs(M) > length(x) % shift amount greater ther vector length
	M = rem(M,length(x));% only shift by remainder of M by length
end
if M < 0 % vector indexes cannot be negative
	M = M + length(x); % negative shifts are cycled to positive shifts
end
y = [x(M+1:length(x)) x(1:M)]; % rearrange vector x by displacing shifted
% elements to the beginning and follow with rest of elements
