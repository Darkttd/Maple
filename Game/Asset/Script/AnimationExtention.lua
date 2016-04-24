function anim.eyeblink(v, n)
    if (v == nil) then return end
    
	n = n or 1;

	if n > #v then
		n = 1;
	end;

	v[n].layer:Show();

	th.timeout(v[n].keyframe, EyeBlinkingCallback, function()
		v[n].layer:Hide();
		anim.eyeblink(v, n+1);
	end);
end

function anim.cursorblink(parent, target, loopTime)
	if (target == nil) then return end

	loopTime = loopTime or 2.0;

	local cloneCursor = target:Clone();
	cloneCursor:SetName('AnimatedCursorCloned');
	anim.scaleout(cloneCursor, loopTime, -0.5, anim.ease.linear);
	anim.fadeout(cloneCursor, loopTime);
	parent:AddChild(cloneCursor);

	th.timeout(loopTime, function() parent:RemoveChild(cloneCursor); end);
end
