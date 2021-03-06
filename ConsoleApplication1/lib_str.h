

#define CAN_FRAME_DATA_MAX 8

struct CAN_DATA {
	int id;
	char len;
	char buf[8];
	unsigned long tm;
};

#define CAN_FIFO_LEN 64
struct CAN_FIFO {
	int in;
	int out;
	struct CAN_DATA buf[CAN_FIFO_LEN];
};

#define CYC_TM_MAX 8
struct CYC_DATA {
	int id;
	char len;
	char buf[8];
	unsigned long tm[CYC_TM_MAX];
};

#define CYC_FIFO_LEN 64
struct CYC_FIFO {
	int in;
	struct CYC_DATA buf[CYC_FIFO_LEN];
};


//---------------------------
static int str_len(char *str)
{
  char *lp = str;
  while(*lp) {
    lp++;
  }
  return (lp - str);
}

static int str_copy(char *dst,const char *src)
{
  int len =0;
  while(*src != 0) {
    *dst++ = *src++;
    len++;
  }
  *dst = 0;
  return len;
}

static bool is_str_same(char *str1,const char *str2)
{
  while(*str2 != 0) {
    if (*str1++ != *str2++)
      return false;
  }
  return true;
}

static int index_of_char(char *str,char b)
{
  int len = str_len(str);
  for (int i=0;i<len;i++) {
    if (str[i] == b)
      return i;
  }
  return -1;
}

static int index_of_str(char *str1,const char *str2)
{
  int opt_len = str_len(str1) - str_len((char *)str2);
  int ret = -1;
  for (int i=0; i<opt_len; i++) {
    if (is_str_same(&str1[i],str2)) {
      ret = i;
      break;
    }
  }
  return ret;
}


//---------------------------
static bool is_num_ascii(char ascii)
{
  char tmp;
  tmp = ascii;
  if (tmp >= '0' && tmp <= '9') {
    return true;
  } else if (tmp >= 'a' && tmp <= 'f') {
    return true;
  } else if (tmp >= 'A' && tmp <= 'F') {
    return true;
  }
  return false;
}

static int index_of_num(char *str){
  char *lp = str;
  while(!is_num_ascii(*lp)) {
    lp++;
  }
  return (lp - str);
}

static int index_of_no_num(char *str){
  char *lp = str;
  while(is_num_ascii(*lp)) {
    lp++;
  }
  return (lp - str);
}

static int ascii_hex_to_int(char ascii)
{
  char tmp;
  tmp = ascii;
  if (tmp >= '0' && tmp <= '9') {
    tmp = tmp - '0';
  } else if (tmp >= 'a' && tmp <= 'f') {
    tmp = tmp - 'a' + 10;
  } else if (tmp >= 'A' && tmp <= 'F') {
    tmp = tmp - 'A' + 10;
  } else {
    tmp = 0;
  }
  return (int)tmp;
}

static int ascii_dec_to_int(char ascii)
{
  char tmp;
  tmp = ascii;
  if (tmp >= '0' && tmp <= '9') {
    tmp = tmp - '0';
  } else {
    tmp = 0;
  }
  return (int)tmp;
}

static char char_to_ascii_hex(char val)
{
  if (val < 10) {
    return val + '0';
  } else if (val < 16) {
    return val - 10 + 'a';
  } else {
    return '0';
  }
}

static char char_to_ascii_dec(char val)
{
  if (val < 10) {
    return val + '0';
  } else if (val < 16) {
    return val - 10 + 'a';
  } else {
    return '0';
  }
}

//---------------------------
static unsigned long hex_buf_to_long(char *buf)
{
  unsigned long val = 0;
  while (is_num_ascii(*buf)) {
    val <<= 4;
    val += ascii_hex_to_int(*buf);
    buf++;
  }
  return val;
}

static unsigned long dec_buf_to_long(char *buf)
{
  unsigned long val = 0;
  while (is_num_ascii(*buf)) {
    val *= 10;
    val += ascii_dec_to_int(*buf);
    buf++;
  }
  return val;
}

static int long_to_hex_buf(char *buf,unsigned long val)
{
  int num = 0;
  while (val > 0 || num == 0) {
    char tmp = (val & 0xf);
    val >>= 4;
    buf[num++] = char_to_ascii_hex(tmp);
  }
  for (int i=0;i<num/2;i++) {
    char tmp = buf[i];
    buf[i] = buf[num - 1 - i];
    buf[num - 1 - i] = tmp;
  }
  return num;
}

static int long_to_dec_buf(char *buf,unsigned long val)
{
  int num = 0;
  while (val > 0 || num == 0) {
    char tmp = (val % 10);
    val /= 10;
    buf[num++] = char_to_ascii_hex(tmp);
  }
  for (int i=0;i<num/2;i++) {
    char tmp = buf[i];
    buf[i] = buf[num - 1 - i];
    buf[num - 1 - i] = tmp;
  }
  return num;
}

static unsigned long char_to_long(char val)
{
  unsigned long tmp = val;
  return tmp&0xff;
}

static char long_to_char(unsigned long val)
{
  return (char)(val&0xff);
}

static unsigned long int_to_long(int val)
{
  unsigned long tmp = val;
  return tmp&0xffff;
}

int long_to_int(unsigned long val)
{
  return (int)(val&0xffff);
}


//---------------------------
static void can_data_copy(struct CAN_DATA *src,struct CAN_DATA *dst)
{
  dst->id = src->id;
  dst->len = src->len;
  for (int i=0;i<src->len;i++) {
    dst->buf[i] = src->buf[i];
  }
  dst->tm = src->tm;
}

static int can_data_to_buf(char *info,struct CAN_DATA *pCan,bool full)
{
  int index = 0;
  char *lp = info;
  int num;

  num = str_copy(lp," id=");
  index += num;
  lp += num;
  num = long_to_hex_buf(lp,int_to_long(pCan->id));
  index += num;
  lp += num;

  if (full) {
    num = str_copy(lp," len=");
    index += num;
    lp += num;
    num = long_to_dec_buf(lp,char_to_long(pCan->len));
    index += num;
    lp += num;

    num = str_copy(lp," data=");
    index += num;
    lp += num;
    for (int i=0;i<pCan->len;i++) {
      num = long_to_hex_buf(lp,char_to_long(pCan->buf[i]));
      index += num;
      lp += num;
      index++;
      *lp++ = ',';
    }
  }

  num = str_copy(lp," tm=");
  index += num;
  lp += num;
  num = long_to_dec_buf(lp,pCan->tm);
  index += num;
  lp += num;
  *lp = 0;

  return index;
}

static const char can_fix_len = 8;
static const char can_fix_buf[8] = {8,7,6,5,4,3,2,1};
static int buf_to_can_data(char *info,struct CAN_DATA *pCan,bool full)
{
  int index;
  char *lp;
  unsigned long tmp;
  char len = 0;

  index = index_of_str(info,"id=");
  if (index > 0) {
    lp = info + index + 3;
    tmp = hex_buf_to_long(lp);
    pCan->id = long_to_int(tmp);
  } else {
    return -1;
  }

  if (full) {
    index = index_of_str(info,"len=");
    if (index > 0) {
      lp = info + index + 4;
      tmp = dec_buf_to_long(lp);
      len = long_to_char(tmp);
      if (len > CAN_FRAME_DATA_MAX) {
        len = CAN_FRAME_DATA_MAX;
      }
      pCan->len = len;
    } else {
      return -2;
    }

    index = index_of_str(info,"data=");
    if (index > 0) {
      lp = info + index + 5;
      bool flag_find = true;
      for (int i=0;i<len;i++) {
        if(flag_find) {
          for (int j=0;j<2;j++) {
            if (is_num_ascii(*lp))
              break;
            else
              lp++;
          }
          if (!is_num_ascii(*lp)) {
            flag_find = false;
          }
        
          tmp = hex_buf_to_long(lp);
          pCan->buf[i] = long_to_char(tmp);
          for (int j=0;j<2;j++) {
            if (!is_num_ascii(*lp))
              break;
            else
              lp++;
          }
        } else {
          pCan->buf[i] = 0;
        }
      }
    } else {
      return -3;
    }
  } else {
    pCan->len = can_fix_len;
    for (int i=0;i<can_fix_len;i++) {
      pCan->buf[i] = can_fix_buf[i];
    }
  }

  index = index_of_str(info,"tm=");
  if (index > 0) {
    lp = info + index + 3;
    pCan->tm = dec_buf_to_long(lp);
  } else {
    return -4;
  }

  return 0;
}


//---------------------------
static void can_data_fifo_init(struct CAN_FIFO *pBuf)
{
	pBuf->in = 0;
	pBuf->out = 0;
}

static bool can_data_fifo_add(struct CAN_FIFO *pBuf, struct CAN_DATA *pCan)
{
	can_data_copy(pCan, &(pBuf->buf[pBuf->in]));
	pBuf->in++;
	if (pBuf->in >= CAN_FIFO_LEN)
	{
		pBuf->in = 0;
	}
	return (pBuf->in != pBuf->out);
}

static bool can_data_fifo_get(struct CAN_FIFO *pBuf, struct CAN_DATA *pCan)
{
	if (pBuf->in != pBuf->out)
	{
		can_data_copy(&(pBuf->buf[pBuf->out]), pCan);
		pBuf->out++;
		if (pBuf->out >= CAN_FIFO_LEN)
		{
			pBuf->out = 0;
		}
		return true;
	}
	return false;
}


//------------------------------------------
void can_cyc_init(struct CYC_DATA *cyc)
{
	cyc->id = -1;
	for (int i = 0; i < CYC_TM_MAX; i++)
	{
		cyc->tm[i] = 0;
	}
}

void cyc_fifo_init(struct CYC_FIFO *fifo)
{
	fifo->in = 0;
}

bool cyc_fifo_add(struct CYC_FIFO *fifo, struct CAN_DATA *can)
{
	int index = -1;
	for (int i = 0; i < fifo->in; i++)
	{
		if (fifo->buf[i].id == can->id)
		{
			index = i;
		}
	}
	if (index < 0 && fifo->in < CYC_FIFO_LEN)
	{
		struct CYC_DATA *lp;
		lp = &(fifo->buf[fifo->in]);
		can_cyc_init(lp);
		lp->id = can->id;
		lp->len = can->len;
		for (int i = 0; i < can->len; i++)
		{
			lp->buf[i] = can->buf[i];
		}
		lp->tm[0] = can->tm;
		fifo->in++;
		return true;
	}
	else
	{
		struct CYC_DATA *lp;
		lp = &(fifo->buf[index]);
		for (int i = CYC_TM_MAX - 1; i > 0; i--)
		{
			lp->tm[i] = lp->tm[i - 1];
		}
		lp->tm[0] = can->tm;
		lp->len = can->len;
		for (int i = 0; i < can->len; i++)
		{
			lp->buf[i] = can->buf[i];
		}
		return true;
	}
	return false;
}

bool cyc_fifo_read(struct CYC_FIFO *fifo, int index, struct CAN_DATA *can)
{
	if (index >= fifo->in)
	{
		return false;
	}
	struct CYC_DATA *lp;
	lp = &(fifo->buf[index]);
	can->id = lp->id;
	can->len = lp->len;
	for (int i = 0; i < lp->len; i++)
	{
		can->buf[i] = lp->buf[i];
	}
	int sum = -1;
	for (int i = 0; i < CYC_TM_MAX; i++)
	{
		if (lp->tm[i] > 0)
			sum++;
	}
	if (sum < 2)
	{
		can->tm = 0;
	}
	else
	{
		can->tm = (lp->tm[0] - lp->tm[sum]) / sum;
	}
	return true;
}


