/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma once
void icache_invalidate_way(void);
void dcache_invalidate_way(void);
void CacheDisable (void);
void CacheEnable (void);
void CacheInit(void);
