/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   States.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:31:39 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/09 15:06:01 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientInfo.hpp"


enum states
{
    READ_REQUEST,
    PROCESS_REQUEST,
    WRITE_RESPONSE
};
// class States
// {
//     public:
//         virtual void handleState( ClientInfo *client ) = 0;                
//         void    reset( ClientInfo *client );
// };