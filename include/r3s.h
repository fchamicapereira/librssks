#ifndef __R3S_API_H__
#define __R3S_API_H__

/** \file */ 

#include <stdint.h>
#include <stdbool.h>

#include <z3.h>

/**
   \defgroup capi C API
*/
/// \{

/**
 * \brief Used for documentation.
 * It's an explicit indication that the parameter
 * prefixed with this keyword is going to be used
 * as an output parameter.
*/
#define out

//! \brief RSS hash output in bytes
#define HASH_OUTPUT_SIZE        4

//! \brief RSS key size in bytes
#define KEY_SIZE        52

//! \brief RSS hash output size in bits
#define HASH_OUTPUT_SIZE_BITS   (HASH_OUTPUT_SIZE * 8)

//! \brief RSS key size in bits
#define KEY_SIZE_BITS   (KEY_SIZE * 8)

//! \brief From RSS hash to core assoginment
#define HASH_TO_CORE(hash, cores)   (((hash) & 0x1ff) % (cores))


/** \name Types */
/// \{

//! \brief Byte used all over this library.
typedef unsigned char R3S_byte_t;

//! \brief Array of bytes type.
typedef R3S_byte_t* R3S_bytes_t;

//! \brief RSS hash input type.
typedef R3S_bytes_t R3S_key_hash_in_t;

//! \brief RSS key type.
typedef R3S_byte_t R3S_key_t[KEY_SIZE];

//! \brief RSS hash output type.
typedef uint32_t R3S_key_hash_out_t;

//! \brief IPv6 packet field type.
typedef R3S_byte_t R3S_ipv6_t[16];

//! \brief IPv4 packet field type.
typedef R3S_byte_t R3S_ipv4_t[4];

//! \brief SCTP verification tag packet field type.
typedef R3S_byte_t R3S_v_tag_t[4];

//! \brief VXLAN segment's network identifier type.
typedef R3S_byte_t R3S_vni_t[3];

//! \brief L4 port type.
typedef R3S_byte_t R3S_port_t[2];

//! \brief L2 protocol type.
typedef R3S_byte_t R3S_ethertype_t[1];

/**
 * \typedef R3S_in_cfg_t
 * Used to infer if a given packet field is loaded.
 * \see R3S_pf_t
 */
typedef unsigned R3S_in_cfg_t;

/**
 * \enum R3S_opt_t
 * Input options typically associated with certains NICs.
 */
typedef enum
{
    /**
     * \todo Which packets?
     * - ::R3S_PF_VXLAN_UDP_OUTER
     * - ::R3S_PF_VXLAN_VNI
     */
    R3S_OPT_GENEVE_OAM,

    /**
     * \todo Which packets?
     * - ::R3S_PF_VXLAN_UDP_OUTER
     * - ::R3S_PF_VXLAN_VNI
     */
    R3S_OPT_VXLAN_GPE_OAM,

    /**
     * Configures RSS to accept non fragmented TCP/IPv4 packets.
     * - ::R3S_PF_IPV4_SRC
     * - ::R3S_PF_IPV4_DST
     * - ::R3S_PF_TCP_SRC
     * - ::R3S_PF_TCP_DST
     */
    R3S_OPT_NON_FRAG_IPV4_TCP,

    /**
     * Configures RSS to accept non fragmented UDP/IPv4 packets.
     * - ::R3S_PF_IPV4_SRC
     * - ::R3S_PF_IPV4_DST
     * - ::R3S_PF_UDP_SRC
     * - ::R3S_PF_UDP_DST
     */
    R3S_OPT_NON_FRAG_IPV4_UDP,

    /**
     * Configures RSS to accept non fragmented SCTP/IPv4 packets.
     * - ::R3S_PF_IPV4_SRC
     * - ::R3S_PF_IPV4_DST
     * - ::R3S_PF_SCTP_SRC
     * - ::R3S_PF_SCTP_DST
     * - ::R3S_PF_SCTP_V_TAG
     */
    R3S_OPT_NON_FRAG_IPV4_SCTP,

    /**
     * Configures RSS to accept non fragmented IPv4 packets.
     * - ::R3S_PF_IPV4_SRC
     * - ::R3S_PF_IPV4_DST
     */
    R3S_OPT_NON_FRAG_IPV4,

    /**
     * Configures RSS to accept fragmented IPv4 packets.
     * - ::R3S_PF_IPV4_SRC
     * - ::R3S_PF_IPV4_DST
     */
    R3S_OPT_FRAG_IPV4,

    /**
     * Configures RSS to accept non fragmented TCP/IPv6 packets.
     * - ::R3S_PF_IPV6_SRC
     * - ::R3S_PF_IPV6_DST
     * - ::R3S_PF_TCP_SRC
     * - ::R3S_PF_TCP_DST
     */
    R3S_OPT_NON_FRAG_IPV6_TCP,

    /**
     * Configures RSS to accept non fragmented UDP/IPv6 packets.
     * - ::R3S_PF_IPV6_SRC
     * - ::R3S_PF_IPV6_DST
     * - ::R3S_PF_UDP_SRC
     * - ::R3S_PF_UDP_DST
     */
    R3S_OPT_NON_FRAG_IPV6_UDP,

    /**
     * Configures RSS to accept non fragmented SCTP/IPv6 packets.
     * - ::R3S_PF_IPV6_SRC
     * - ::R3S_PF_IPV6_DST
     * - ::R3S_PF_SCTP_SRC
     * - ::R3S_PF_SCTP_DST
     * - ::R3S_PF_SCTP_V_TAG
     */
    R3S_OPT_NON_FRAG_IPV6_SCTP,

    /**
     * Configures RSS to accept non fragmented IPv6 packets.
     * - ::R3S_PF_IPV6_SRC
     * - ::R3S_PF_IPV6_DST
     */
    R3S_OPT_NON_FRAG_IPV6,

    /**
     * Configures RSS to accept fragmented IPv6 packets.
     * - ::R3S_PF_IPV6_SRC
     * - ::R3S_PF_IPV6_DST
     */
    R3S_OPT_FRAG_IPV6,

    /**
      * Configures RSS to accept *all* packets.
      * - ::R3S_PF_ETHERTYPE
      */
    R3S_OPT_ETHERTYPE
} R3S_opt_t;

// Undocumented and not really important
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define R3S_FIRST_PF R3S_PF_VXLAN_UDP_OUTER
    #define R3S_LAST_PF  R3S_PF_ETHERTYPE

    #define R3S_FIRST_OPT R3S_OPT_GENEVE_OAM
    #define R3S_LAST_OPT  R3S_OPT_ETHERTYPE
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/**
 * \enum R3S_pf_t
 * Packet fields loaded into the RSS configuration, i.e.,
 * the packet fields that will be given to the hash function.
 * 
 * The order is important!
 * From top to bottom, if one field is enumerated first, then
 * it is placed first on the hash input.
 * 
 * Eg, if one configured the hash to accept ipv4 src and dst,
 * and tcp src and dst, then the hash input would be
 * { ipv4_src, ipv4_dst, tcp_src, tcp_dst }.
 */
typedef enum
{

    R3S_PF_VXLAN_UDP_OUTER, /*!< UDP outer used in the VXLAN protocol. */
    R3S_PF_VXLAN_VNI,       /*!< VXLAN network identifier used in the VXLAN protocol. */

    R3S_PF_IPV6_SRC,        /*!< IPv6 source address. */
    R3S_PF_IPV6_DST,        /*!< IPv6 destination address. */

    R3S_PF_IPV4_SRC,        /*!< IPv4 source address. */
    R3S_PF_IPV4_DST,        /*!< IPv4 destination address. */

    R3S_PF_TCP_SRC,         /*!< TCP source port. */
    R3S_PF_TCP_DST,         /*!< TCP destination port. */

    R3S_PF_UDP_SRC,         /*!< UDP source port. */
    R3S_PF_UDP_DST,         /*!< UDP destination port. */

    R3S_PF_SCTP_SRC,        /*!< SCTP source port. */
    R3S_PF_SCTP_DST,        /*!< SCTP destination port. */
    R3S_PF_SCTP_V_TAG,      /*!< SCTP verification tag. */

    R3S_PF_ETHERTYPE,       /*!< Layer 2 protocol. */

} R3S_pf_t;

/**
 * \enum R3S_status_t
 * Status code used as return values.
 */
typedef enum
{
    R3S_STATUS_SUCCESS,         /*!< Operation finished successefully. */
    R3S_STATUS_NO_SOLUTION,     /*!< No solution was found. */
    R3S_STATUS_BAD_SOLUTION,    /*!< A solution was found, but it doesn't fill the requirements. */
    R3S_STATUS_HAS_SOLUTION,    /*!< Confirmation that a solution exists. */
    
    R3S_STATUS_PF_UNKNOWN,      /*!< Unknown packet field. */
    R3S_STATUS_PF_LOADED,       /*!< Packet field loaded into configuration. */
    R3S_STATUS_PF_NOT_LOADED,   /*!< Packet field not loaded into configuration. */
    R3S_STATUS_PF_INCOMPATIBLE, /*!< Packet field incompatible with the current configuration. */

    R3S_STATUS_OPT_UNKNOWN,     /*!< Unknown configuration option. */
    R3S_STATUS_OPT_LOADED,      /*!< Option loaded into configuration. */
    R3S_STATUS_OPT_NOT_LOADED,  /*!< Option not loaded into configuration. */
    R3S_STATUS_INVALID_IOPT,    /*!< Invalid option index. This is typically returned if an used option index is bigger than the number of loaded options in a configuration. */
    
    R3S_STATUS_FAILURE          /*!< Operation failed. */
} R3S_status_t;

/**
 * \struct R3S_packet_t
 * \brief Packet associated with an RSS configuration.
 */
typedef struct
{
    R3S_in_cfg_t cfg; //!< Packet field configuration, i.e., which packet fields are being used.

    union
    {
        //! \unnamed{union}
        R3S_ethertype_t ethertype; //!< L2 protocol.
    };

    union
    {
        struct
        {
            //! \unnamed{union/struct:2}
            R3S_ipv4_t src;
            R3S_ipv4_t dst;
        } ipv4; //!< IPv4 header.
        
        struct
        {
            //! \unnamed{union/struct:2}
            R3S_ipv6_t src;
            R3S_ipv6_t dst;
        } ipv6; //!< IPv6 header.
    };

    union
    {
        struct
        {
            //! \unnamed{union/struct:2}
            R3S_port_t src;
            R3S_port_t dst;
        } tcp; //!< TCP header.
        
        struct
        {
            //! \unnamed{union/struct:2}
            R3S_port_t src;
            R3S_port_t dst;
        } udp; //!< UDP header.

        struct
        {
            //! \unnamed{union/struct:3}
            R3S_port_t  src;
            R3S_port_t  dst;
            R3S_v_tag_t tag;
        } sctp; //!< SCTP header.
    };

    union
    {
        struct
        {
            //! \unnamed{union/struct:2}
            R3S_port_t outer;
            R3S_vni_t  vni;
        } vxlan; //!< VXLAN header.
    };
} R3S_packet_t;

/**
 * \struct R3S_loaded_opt_t
 * \brief Information regarding loaded option and consequently the associated
 * packet fields and the size of the hash input.
 */
typedef struct {
    R3S_opt_t opt;      //!< Loaded option.
    R3S_in_cfg_t pfs;   //!< Hash input configuration, i.e., chosen packet fields associated with this option.
    unsigned     sz;    //!< Size of the hash input.
} R3S_loaded_opt_t;

/**
 * \struct R3S_cfg_t
 * \brief R3S configuration used to store information useful
 * throughout the API.
 */
typedef struct {

    /**
     *Options loaded in this configuration.
     * \see R3S_opt_t
     */
    R3S_loaded_opt_t *loaded_opts;
    
    /**
     * Number of loaded configurations. Stores the size of
     * the R3S_cfg_t::loaded_opts array. 
     */
    unsigned         n_loaded_opts;

    /**
     * Number of processes to be used by the R3S_keys_fit_cnstrs().
     * If this value is <= 0, then the number of processes
     * used will be equal to the number of available cores.
     */
    int              n_procs;

    /**
     * Number of keys to take into consideration.
     * This is useful when there are constraints needed to be
     * considered between multiple NICs/ports in NICs.
     */
    unsigned         n_keys;

    struct {
        char         *pcap_fname;
        float        std_dev_threshold;
        int          time_limit;
        unsigned     n_cores;
    } key_fit_params;
} R3S_cfg_t;

/**
 * \brief Definition of the function used to represent constraints between packets.
 * 
 * \param cfg R3S configuration
 * \param iopt Index of the option 
 * \param ctx Z3 context
 * \param p1 A packet
 * \param p2 Another packet
 * \see R3S_keys_fit_cnstrs()
 * 
 * \code
 * Z3_ast R3S_cnstr_symmetric_ip(R3S_cfg_t r3s_cfg, unsigned iopt, Z3_context ctx, Z3_ast p1, Z3_ast p2)
 * {
 *     R3S_status_t status;
 *     Z3_ast       p1_ipv4_src, p1_ipv4_dst;
 *     Z3_ast       p2_ipv4_src, p2_ipv4_dst;
 *     Z3_ast       and_args[2];
 *     
 *     status = R3S_packet_extract_pf(r3s_cfg, iopt, ctx, p1, R3S_PF_IPV4_SRC, &p1_ipv4_src);
 *     if (status != R3S_STATUS_SUCCESS) return NULL;
 *     
 *     status = R3S_packet_extract_pf(r3s_cfg, iopt, ctx, p1, R3S_PF_IPV4_DST, &p1_ipv4_dst);
 *     if (status != R3S_STATUS_SUCCESS) return NULL;
 *     
 *     status = R3S_packet_extract_pf(r3s_cfg, iopt, ctx, p2, R3S_PF_IPV4_SRC, &p2_ipv4_src);
 *     if (status != R3S_STATUS_SUCCESS) return NULL;
 *     
 *     status = R3S_packet_extract_pf(r3s_cfg, iopt, ctx, p2, R3S_PF_IPV4_DST, &p2_ipv4_dst);
 *     if (status != R3S_STATUS_SUCCESS) return NULL;
 *     
 *     and_args[0] = Z3_mk_eq(ctx, p1_ipv4_src, p2_ipv4_dst);
 *     and_args[1] = Z3_mk_eq(ctx, p1_ipv4_dst, p2_ipv4_src);
 *     
 *     return Z3_mk_and(ctx, 2, and_args);
 * }
 * \endcode
 */
typedef Z3_ast (*R3S_cnstrs_func)(R3S_cfg_t cfg,unsigned iopt,Z3_context ctx,Z3_ast p1,Z3_ast p2);


/**
 * \struct R3S_core_stats_t
 * \brief Number of packets redirected to a single core, and its percentage.
 */
typedef struct
{
    unsigned n_packets; //!< Total number of packets redirected to a single core.
    float percentage;   //!< Percentage of the total number of packets that was redirected to a single core.
} R3S_core_stats_t;

/**
 * \struct R3S_stats_t
 * \brief Key statistics.
 */
typedef struct {
    R3S_cfg_t           cfg;         //!< R3S configuration.
    R3S_core_stats_t    *core_stats; //!< Statistics related to each core.
    unsigned            n_cores;     //!< Total number of cores to be considered.
    float               avg_dist;    //!< Average distribution of packets per core (in percentage).
    float               std_dev;     //!< Standard deviation of the distribution of packets per core (in percentage).

} R3S_stats_t;

/**
 * \brief R3S boilerplate string.
 * \note There is no need to worry about freeing memory when using this.
 */
typedef char* R3S_string_t;

/// \}

/** \name String conversion */
/// \{

/**
 * \brief Get the string representation of a key.
 * 
 * \param k Key
 * \return ::R3S_string_t String representation of \p k.
 */
R3S_string_t R3S_key_to_string(R3S_key_t k);

/**
 * \brief Get the string representation of a packet.
 * 
 * \param p Packet
 * \return ::R3S_string_t String representation of \p p.
 */
R3S_string_t R3S_packet_to_string(R3S_packet_t p);

/**
 * \brief Get the string representation of a hash output.
 * 
 * \param o Hash output
 * \return ::R3S_string_t String representation of \p o.
 */
R3S_string_t R3S_key_hash_output_to_string(R3S_key_hash_out_t o);

/**
 * \brief Get the string representation of a status code.
 * 
 * \param s Status
 * \return ::R3S_string_t String representation of \p s.
 */
R3S_string_t R3S_status_to_string(R3S_status_t s);

/**
 * \brief Get the string representation of a configuration option.
 * 
 * \param opt Option
 * \return ::R3S_string_t String representation of \p opt.
 */
R3S_string_t R3S_opt_to_string(R3S_opt_t opt);

/**
 * \brief Get the string representation of a packet field.
 * 
 * \param pf Packet field
 * \return ::R3S_string_t String representation of \p pf.
 */
R3S_string_t R3S_pf_to_string(R3S_pf_t pf);

/**
 * \brief Get the string representation of a configuration.
 * 
 * \param cfg Configuration
 * \return ::R3S_string_t String representation of \p cfg.
 */
R3S_string_t R3S_cfg_to_string(R3S_cfg_t cfg);

/**
 * \brief Get the string representation of key statistics.
 * 
 * \param stats Statistics.
 * \return ::R3S_string_t String representation of \p stats.
 */
R3S_string_t R3S_stats_to_string(R3S_stats_t stats);

/// \}

/** \name Configuration */
/// \{

/**
 * \brief Initialize a configuration.
 * \param cfg Configuration to initialize.
 */
void R3S_cfg_init(out R3S_cfg_t *cfg);

/**
 * \brief Reset a configuration.
 * \param cfg Configuration to reset.
 */
void R3S_cfg_reset(out R3S_cfg_t *cfg);

/**
 * \brief Delete a configuration.
 * \param cfg Configuration to delete.
 */
void R3S_cfg_delete(out R3S_cfg_t *cfg);

/**
 * \brief Load option into configuration.
 * \param cfg Configuration to modify.
 * \param opt Option to load.
 */
R3S_status_t R3S_cfg_load_opt(out R3S_cfg_t *cfg, R3S_opt_t opt);

/// \}

/** \name Packet*/
/// \{

/**
 * Initialize packet.
 * 
 * \param p Packet.
 */
void R3S_packet_init(R3S_packet_t *p);

/**
 * Set packet field in packet.
 * 
 * \param cfg R3S configuration.
 * \param pf Type of packet field to store.
 * \param v Value of the packet field.
 * \param p Pointer to a packet with the value set.
 * 
 * \return ::R3S_STATUS_SUCCESS
 * Packet *p* with the packet field *pf* set with value *v*.
 * 
 * \return ::R3S_STATUS_PF_INCOMPATIBLE
 * Packet field isn't compatible with the current configuration. Packet *p* unchanged.
 */
R3S_status_t R3S_packet_set_pf(R3S_cfg_t cfg, R3S_pf_t pf, R3S_bytes_t v, R3S_packet_t *p);
R3S_status_t R3S_packet_set_ethertype(R3S_cfg_t cfg, R3S_ethertype_t ethertype, out R3S_packet_t *p);
R3S_status_t R3S_packet_set_ipv4(R3S_cfg_t cfg, R3S_ipv4_t src, R3S_ipv4_t dst, out R3S_packet_t *p);
R3S_status_t R3S_packet_set_ipv6(R3S_cfg_t cfg, R3S_ipv6_t src, R3S_ipv6_t dst, out R3S_packet_t *p);
R3S_status_t R3S_packet_set_tcp(R3S_cfg_t cfg, R3S_port_t src, R3S_port_t dst, out R3S_packet_t *p);
R3S_status_t R3S_packet_set_udp(R3S_cfg_t cfg, R3S_port_t src, R3S_port_t dst, out R3S_packet_t *p);
R3S_status_t R3S_packet_set_sctp(R3S_cfg_t cfg, R3S_port_t src, R3S_port_t dst, R3S_v_tag_t tag, out R3S_packet_t *p);
R3S_status_t R3S_packet_set_vxlan(R3S_cfg_t cfg, R3S_port_t outer, R3S_vni_t vni, out R3S_packet_t *p);

R3S_status_t R3S_packet_from_cnstrs(R3S_cfg_t r3s_cfg, R3S_packet_t p, R3S_cnstrs_func mk_p_cnstrs, out R3S_packet_t *result);
R3S_status_t R3S_packet_extract_pf(R3S_cfg_t r3s_cfg, unsigned iopt, Z3_context ctx, Z3_ast p, R3S_pf_t pf, out Z3_ast *result);
R3S_status_t R3S_packets_parse(R3S_cfg_t cfg, char* filename, out R3S_packet_t **packets, int *n_packets);
R3S_status_t R3S_packet_rand(R3S_cfg_t cfg, out R3S_packet_t *p);
R3S_status_t R3S_packets_rand(R3S_cfg_t cfg, unsigned n_packets, out R3S_packet_t **p);
/// \}


/** \name Key statistics and evaluation */
/// \{

void R3S_stats_init(R3S_cfg_t cfg, unsigned n_cores, out R3S_stats_t *stats);
void R3S_stats_reset(R3S_cfg_t cfg, unsigned n_cores, out R3S_stats_t *stats);
void R3S_stats_delete(out R3S_stats_t *stats);
R3S_status_t R3S_stats_from_packets(R3S_key_t key, R3S_packet_t *packets, int n_packets, out R3S_stats_t *stats);
bool R3S_stats_eval(R3S_cfg_t cfg, R3S_key_t key, out R3S_stats_t *stats);

/// \}

/** \name Key */
/// \{

/**
 * \brief Randomize a key.
 * \param cfg R3S configuration.
 * \param key Key to randomize.
 */
void R3S_key_rand(R3S_cfg_t cfg, out R3S_key_t key);

/**
 * \brief Find keys that fit the given constraints, and insert them
 * in the parameter array \p keys.
 * 
 * The array \p keys must be allocated beforehand, and its size
 * is specified in the R3S_cfg_t input parameter, using
 * its R3S_cfg_t::n_keys field.
 * 
 * The constraints are represented using a function with the definition
 * R3S_cnstrs_func (check its documentation).
 * 
 * The first n = r3s_cfg.n_keys elements of \p mk_p_cnstrs relate to
 * constraints on each key independently. The remaining elements
 * correspond to the constraints related to combinations of keys.
 * 
 *  index  | keys | description
 * ------- | -------------|-------------
 *    0    | k[0]         | Constraints for the first key
 *    1    | k[1]         | Constraints for the second key
 *   ...   | ...          | ...
 *   n-1   | k[n-1]       | Constraints for the last key
 *    n    | k[0], k[1]   | Constraints for the first and second keys
 *   n+1   | k[0], k[2]   | Constraints for the first and third keys
 *   ...   | ...          | ...
 *  2n-1   | k[0], k[n-1] | Constraints for the first and last keys
 *   2n    | k[1], k[2]   | Constraints for the second and third keys
 *   ...   | ...          | ...
 *   
 * 
 * Considering \f$ \binom{n}{m} \f$ as combinations of \f$ n \f$, \f$ m \f$
 * by \f$ m \f$, the size of \p mk_p_cnstrs must be at least
 * \f$ n + \binom{n}{2} \f$. This condition is
 * checked within this function, and it fails if it isn't met.
 * 
 * For example, using r3s_cfg.n_keys = 3, and knowing that
 * \f$ \binom{3}{2} = 3 \f$:
 *   - mk_p_cnstrs[0]    => constraints on k[0]
 * 
 *   - mk_p_cnstrs[1]    => constraints on k[1]
 * 
 *   - mk_p_cnstrs[2]    => constraints on k[2]
 * 
 *   - mk_p_cnstrs[3]    => constraints between k[0] and k[1]
 * 
 *   - mk_p_cnstrs[4]    => constraints between k[0] and k[2]
 * 
 *   - mk_p_cnstrs[5]    => constraints between k[1] and k[2]
 * 
 * 
 * \param r3s_cfg R3S configuration containing the number of keys to be used.
 * \param mk_p_cnstrs Function used to represent constraints between packets.
 * \param keys Array of generated keys that respect the constraints given.
 * 
 * \see R3S_cnstrs_func
 * 
 * \return Status code.
 */
R3S_status_t R3S_keys_fit_cnstrs(R3S_cfg_t r3s_cfg, R3S_cnstrs_func *mk_p_cnstrs, out R3S_key_t *keys);
/** 
 * \example no_solution.c
 * Consider the following scenario:
 * *find an RSS key that for every pair of IPv4 packets p1 and p2,
 * if the IP source address of the packet p1 is equal to itself (i.e.,
 * always), then these two packets must have the same hash value*.
 * 
 * This example shows that there is no key that satisfied these
 * constraints.
 */
/** 
 * \example src_ip.c
 * \todo explanation
 */
/** 
 * \example symmetric_ip_and_symmetric_session.c
 * \todo explanation
 */
/** 
 * \example symmetric_ip.c
 * \todo explanation
 */
/** 
 * \example symmetric_session_2_cnstrs_diff_pf.c
 * \todo explanation
 */
/** 
 * \example symmetric_session_2_cnstrs_eq_pf.c
 * \todo explanation
 */
/** 
 * \example symmetric_session.c
 * \todo explanation
 */

R3S_status_t R3S_keys_test_cnstrs(R3S_cfg_t r3s_cfg, R3S_cnstrs_func *mk_p_cnstrs, out R3S_key_t *keys);

R3S_status_t R3S_key_hash(R3S_cfg_t cfg, R3S_key_t k, R3S_packet_t h, out R3S_key_hash_out_t *result);

/// \}

/** \name Constraints */
/// \{

//void R3S_cnstrs_test(R3S_cfg_t r3s_cfg, R3S_cnstrs_func mk_p_cnstrs, R3S_packet_t h1, R3S_packet_t h2);
Z3_ast R3S_cnstr_symmetric_ip(R3S_cfg_t r3s_cfg, unsigned iopt, Z3_context ctx, Z3_ast p1, Z3_ast p2);
Z3_ast R3S_cnstr_symmetric_tcp(R3S_cfg_t r3s_cfg, unsigned iopt, Z3_context ctx, Z3_ast p1, Z3_ast p2);
Z3_ast R3S_cnstr_symmetric_tcp_ip(R3S_cfg_t r3s_cfg, unsigned iopt, Z3_context ctx, Z3_ast p1, Z3_ast p2);

/// \}
/// \}

#endif