
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>


#define NETLINK_TEST 19   //Hello
#define MAX_PAYLOAD  2048 /* maximum payload size*/

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd = 0;
struct msghdr msg;

void main()
{
  sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST);
  memset(&src_addr, 0, sizeof(src_addr));
  src_addr.nl_family = AF_NETLINK;
  src_addr.nl_pid = getpid(); /* self pid */
  src_addr.nl_groups = 0; /* not in mcast groups */
  bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.nl_family = AF_NETLINK;
  dest_addr.nl_pid = 0;   /* For Linux Kernel */
  dest_addr.nl_groups = 0; /* unicast */
  nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
  /* Fill the netlink message header */
  nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
  nlh->nlmsg_pid = getpid(); /* self pid */
  nlh->nlmsg_flags = 0;
  /* Fill in the netlink message payload */
  strcpy(NLMSG_DATA(nlh), "Sudhansu : User space !!!");
  iov.iov_base = (void *)nlh;
  iov.iov_len = nlh->nlmsg_len;
  msg.msg_name = (void *)&dest_addr;
  msg.msg_namelen = sizeof(dest_addr);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  sendmsg(sock_fd, &msg, 0);
  
  while (1)
  {
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    recvmsg(sock_fd, &msg, 0);
    printf("\n");
    printf("Received data from  Kernel : [%s]", NLMSG_DATA(nlh));
    printf("\n");
  }
  
  /* Close Netlink Socket */
  close(sock_fd);
}

