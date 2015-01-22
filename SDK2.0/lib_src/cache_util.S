.file "cache_utils.S"
#define CACH_ON_VAL 0x77

/* void icache_invalidate_way(void)*/
	.section .text
    .global _icache_invalidate_way
	.type _icache_invalidate_way, @function
_icache_invalidate_way:	
/*cache disable*/
	push	%r0
	set		8
	ldi		0x0,%r0
	mvtc 	0,%r9
/*waybase invalidation	writeback*/
	ldi		0xc,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1c,%r0
	mvtc	0,%R11
	sync
	
/*cache on	*/
	ldi		CACH_ON_VAL,%r0
	mvtc 	0,%r9
	sync
	clr 	8
	pop		%r0
	jplr
.size _icache_invalidate_way, .-_icache_invalidate_way	


/* void dcache_invalidate_way(void)*/
	.section .ispm,"ax",@progbits
	.align 1
    .global _dcache_invalidate_way
	.type _dcache_invalidate_way, @function
_dcache_invalidate_way:	
/*cache disable*/
	set		8
	sync
	push	%r0
	push 	%r1

	ldi 	0x0,%r1
	mvfc	0,%r9
	sync
	cmp		%r0,%r1
	jz		_enddcache_
	
/* cache off */	
	ldi		0x0,%r0
	mvtc 	0,%r9
	sync
/*waybase invalidation	writeback*/
	ldi		0xd,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1d,%r0
	mvtc	0,%R11
	sync
	
/*waybase invalidation	writeback*/
	ldi		0xc,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1c,%r0
	mvtc	0,%R11
	sync

	
/*cache on	*/
	ldi		CACH_ON_VAL,%r0
	mvtc 	0,%r9
	sync
_enddcache_	:
	pop		%r1
	pop		%r0
	clr		8
	sync
	jplr
.size _dcache_invalidate_way, .-_dcache_invalidate_way	

	
/*	void CacheDisable (void)*/
	.global _CacheDisable
	.type _CacheDisable, @function
_CacheDisable:
/*cache disable*/
	push	%r0
	set 	8
	ldi		0x0,%r0
	mvtc 	0,%r9
	sync
/*i-cache waybase invalidation	 writeback*/
	ldi		0xc,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1c,%r0
	mvtc	0,%R11
	sync

/*d-cache waybase invalidation	writeback*/
	ldi		0xd,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1d,%r0
	mvtc	0,%R11
	sync
	
	clr 	8
	pop		%r0
	jplr

.size _CacheDisable, .-_CacheDisable	

/*	void CacheEnable (void)*/
	.global _CacheEnable
	.type _CacheEnable, @function
_CacheEnable:
	push	%r0
	set 	8
	ldi		CACH_ON_VAL,%r0
	mvtc 	0,%r9
	sync
	
	clr 	8
	pop		%r0
	jplr

.size _CacheEnable, .-_CacheEnable	


/* void CacheInit(void)*/
	.section .text
    .global _CacheInit
	.type _CacheInit, @function
_CacheInit:	
	push %r0
	set 	8
/* if cache on */
	mvfc	0,%r9
	sync
	cmp		0,%r0
	jnz		._idinvalidation_copyback_:

._idinvalidation_:
/*i-cache waybase invalidation	*/	
	ldi		0x8,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x18,%r0
	mvtc	0,%R11
	sync
/*d-cache waybase invalidation */
	ldi		0x9,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x19,%r0
	mvtc	0,%R11
	sync

/*cache on	*/
	ldi		CACH_ON_VAL,%r0
	mvtc 	0,%r9
	sync
	
	clr 	8
	pop		%r0
	jplr

._idinvalidation_copyback_	:
/* cache off */
	ldi		0x0,%r0
	mvtc 	0,%r9
	sync
/*i-cache waybase invalidation	writeback*/	
	ldi		0xc,%r0
	mvtc	0,%R11
	sync
	
	ldi		0x1c,%r0
	mvtc	0,%R11
	sync
	
/*cache on	*/
	ldi		CACH_ON_VAL,%r0
	mvtc 	0,%r9
	sync
	
	clr 8
	pop		%r0
	jplr
.size _CacheInit, .-_CacheInit	

