function _cloak_spec_5
set 1 $argv[1]
__fish_complete_path "$1"
end

function _cloak
    set COMP_LINE (commandline --cut-at-cursor)

    set COMP_WORDS
    echo $COMP_LINE | read --tokenize --array COMP_WORDS
    if string match --quiet --regex '.*\s$' $COMP_LINE
        set COMP_CWORD (math (count $COMP_WORDS) + 1)
    else
        set COMP_CWORD (count $COMP_WORDS)
    end

    set literals "--key" "--help" "-i" "--gitignore-ignore" "-k" "encrypt" "decrypt" "ls" "hash" "-G" "--output" "-o" "-v" "--version" "-h" "--input"

    set descriptions
    set descriptions[3] "Specify the input path"
    set descriptions[5] "Specify the encryption key path"
    set descriptions[6] "Encrypt a file"
    set descriptions[7] "Decrypt a file"
    set descriptions[8] "Lists the files considered in the encryption process"
    set descriptions[9] "Prints the hash of a file"
    set descriptions[10] "Do not include the gitignore patterns alongside cloakignore"
    set descriptions[12] "Specify the output path"
    set descriptions[13] "Prints the project version"
    set descriptions[15] "Prints the help message"

    set literal_transitions
    set literal_transitions[1] "set inputs 2 13 15 6 7 8 9 14; set tos 2 2 2 3 4 5 6 2"
    set literal_transitions[3] "set inputs 3 16; set tos 13 13"
    set literal_transitions[4] "set inputs 3 16; set tos 15 15"
    set literal_transitions[5] "set inputs 10 3 4 16; set tos 2 7 2 7"
    set literal_transitions[6] "set inputs 3 16; set tos 10 10"
    set literal_transitions[8] "set inputs 4 10; set tos 2 2"
    set literal_transitions[9] "set inputs 1 11 5 12; set tos 10 11 10 11"
    set literal_transitions[12] "set inputs 11 10 4 12; set tos 7 2 2 7"
    set literal_transitions[14] "set inputs 1 5; set tos 10 10"

    set match_anything_transitions_from 10 11 7 13 15
    set match_anything_transitions_to 2 14 8 12 9

    set state 1
    set word_index 2
    while test $word_index -lt $COMP_CWORD
        set -- word $COMP_WORDS[$word_index]

        if set --query literal_transitions[$state] && test -n $literal_transitions[$state]
            set --erase inputs
            set --erase tos
            eval $literal_transitions[$state]

            if contains -- $word $literals
                set literal_matched 0
                for literal_id in (seq 1 (count $literals))
                    if test $literals[$literal_id] = $word
                        set index (contains --index -- $literal_id $inputs)
                        set state $tos[$index]
                        set word_index (math $word_index + 1)
                        set literal_matched 1
                        break
                    end
                end
                if test $literal_matched -ne 0
                    continue
                end
            end
        end

        if set --query match_anything_transitions_from[$state] && test -n $match_anything_transitions_from[$state]
            set index (contains --index -- $state $match_anything_transitions_from)
            set state $match_anything_transitions_to[$index]
            set word_index (math $word_index + 1)
            continue
        end

        return 1
    end

    if set --query literal_transitions[$state] && test -n $literal_transitions[$state]
        set --erase inputs
        set --erase tos
        eval $literal_transitions[$state]
        for literal_id in $inputs
            if test -n $descriptions[$literal_id]
                printf '%s\t%s\n' $literals[$literal_id] $descriptions[$literal_id]
            else
                printf '%s\n' $literals[$literal_id]
            end
        end
    end

    set specialized_command_states 7 10 15 13 11
    set specialized_command_ids 5 5 5 5 5
    if contains $state $specialized_command_states
        set index (contains --index $state $specialized_command_states)
        set function_id $specialized_command_ids[$index]
        set function_name _cloak_spec_$function_id
        set --erase inputs
        set --erase tos
        set lines (eval $function_name $COMP_WORDS[$COMP_CWORD])
        for line in $lines
            printf '%s\n' $line
        end
    end

    return 0
end

complete --command cloak --no-files --arguments "(_cloak)"
