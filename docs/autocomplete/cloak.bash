_cloak_cmd_0 () {
    compgen -A file "$1"
}

_cloak () {
    if [[ $(type -t _get_comp_words_by_ref) != function ]]; then
        echo _get_comp_words_by_ref: function not defined.  Make sure the bash-completions system package is installed
        return 1
    fi

    local words cword
    _get_comp_words_by_ref -n "$COMP_WORDBREAKS" words cword

    declare -a literals=(decrypt rsa -i --gitignore-ignore init ls ini encrypt hash --type --cloakignore-ignore --output -o generate autocomplete --help -v --version signatures --key --input)
    declare -A literal_transitions=()
    literal_transitions[0]="([0]=6 [16]=1 [4]=1 [5]=2 [15]=1 [7]=3 [8]=4 [10]=1 [13]=5 [17]=1)"
    literal_transitions[2]="([3]=12 [2]=21 [10]=14 [9]=13 [20]=21)"
    literal_transitions[3]="([2]=11 [20]=11)"
    literal_transitions[4]="([2]=9 [20]=9)"
    literal_transitions[5]="([9]=10)"
    literal_transitions[6]="([2]=17 [20]=17)"
    literal_transitions[7]="([11]=8 [12]=8 [19]=9)"
    literal_transitions[10]="([1]=1 [6]=1 [14]=1)"
    literal_transitions[12]="([9]=13 [10]=14)"
    literal_transitions[13]="([18]=1)"
    literal_transitions[14]="([9]=13)"
    literal_transitions[15]="([11]=16 [3]=1 [12]=16)"
    literal_transitions[18]="([19]=9)"
    literal_transitions[19]="([3]=1)"
    literal_transitions[20]="([9]=13 [3]=12 [10]=14)"
    declare -A match_anything_transitions=([9]=1 [17]=7 [8]=18 [11]=15 [16]=19 [21]=20)
    declare -A subword_transitions

    local state=0
    local word_index=1
    while [[ $word_index -lt $cword ]]; do
        local word=${words[$word_index]}

        if [[ -v "literal_transitions[$state]" ]]; then
            declare -A state_transitions
            eval "state_transitions=${literal_transitions[$state]}"

            local word_matched=0
            for literal_id in $(seq 0 $((${#literals[@]} - 1))); do
                if [[ ${literals[$literal_id]} = "$word" ]]; then
                    if [[ -v "state_transitions[$literal_id]" ]]; then
                        state=${state_transitions[$literal_id]}
                        word_index=$((word_index + 1))
                        word_matched=1
                        break
                    fi
                fi
            done
            if [[ $word_matched -ne 0 ]]; then
                continue
            fi
        fi

        if [[ -v "match_anything_transitions[$state]" ]]; then
            state=${match_anything_transitions[$state]}
            word_index=$((word_index + 1))
            continue
        fi

        return 1
    done

    declare -A literal_transitions_level_0=([0]="0 16 4 5 15 7 8 10 13" [19]="3" [6]="2" [15]="3 12" [5]="9" [10]="1 6 14" [3]="2" [20]="9 3 10" [2]="3 2 10 9" [14]="9" [12]="9 10" [7]="12 19" [13]="18" [18]="19" [4]="2")
    declare -A literal_transitions_level_1=([15]="11" [4]="20" [3]="20" [2]="20" [0]="17" [7]="11" [6]="20")
    declare -A subword_transitions_level_0=()
    declare -A subword_transitions_level_1=()
    declare -A commands_level_0=()
    declare -A commands_level_1=()
    declare -A specialized_commands_level_0=([17]="0" [16]="0" [9]="0" [21]="0" [11]="0" [8]="0")
    declare -A specialized_commands_level_1=()

    local -a matches=()
    local max_fallback_level=1
    local prefix="${words[$cword]}"
    for (( fallback_level=0; fallback_level <= max_fallback_level; fallback_level++ )) {
       eval "declare literal_transitions_name=literal_transitions_level_${fallback_level}"
       eval "declare -a transitions=(\${$literal_transitions_name[$state]})"
       for literal_id in "${transitions[@]}"; do
           local literal="${literals[$literal_id]}"
           if [[ $literal = "${prefix}"* ]]; then
               matches+=("$literal ")
           fi
       done

       eval "declare subword_transitions_name=subword_transitions_level_${fallback_level}"
       eval "declare -a transitions=(\${$subword_transitions_name[$state]})"
       for subword_id in "${transitions[@]}"; do
           readarray -t -O "${#matches[@]}" matches < <(_cloak_subword_$subword_id complete "$prefix")
       done

       eval "declare commands_name=commands_level_${fallback_level}"
       eval "declare -a transitions=(\${$commands_name[$state]})"
       for command_id in "${transitions[@]}"; do
           local completions=()
           readarray -t completions < <(_cloak_cmd_$command_id "$prefix" | cut -f1)
           for item in "${completions[@]}"; do
               if [[ $item = "${prefix}"* ]]; then
                   matches+=("$item")
               fi
           done
       done

       eval "declare specialized_commands_name=specialized_commands_level_${fallback_level}"
       eval "declare -a transitions=(\${$specialized_commands_name[$state]})"
       for command_id in "${transitions[@]}"; do
           local completions=()
           readarray -t completions < <(_cloak_cmd_"${command_id}" "$prefix" | cut -f1)
           for item in "${completions[@]}"; do
               if [[ $item = "${prefix}"* ]]; then
                   matches+=("$item")
               fi
           done
       done

       if [[ ${#matches[@]} -gt 0 ]]; then
           local shortest_suffix="$prefix"
           for ((i=0; i < ${#COMP_WORDBREAKS}; i++)); do
               local char="${COMP_WORDBREAKS:$i:1}"
               local candidate=${prefix##*$char}
               if [[ ${#candidate} -lt ${#shortest_suffix} ]]; then
                   shortest_suffix=$candidate
               fi
           done
           local superfluous_prefix=""
           if [[ "$shortest_suffix" != "$prefix" ]]; then
               local superfluous_prefix=${prefix%$shortest_suffix}
           fi
           COMPREPLY=("${matches[@]#$superfluous_prefix}")
           break
       fi
    }

    return 0
}

complete -o nospace -F _cloak cloak
